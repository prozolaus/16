#include "Simple_window.h"
#include "Graph.h"

struct Flex_Function : Shape
{
  Flex_Function(Fct f, double r1, double r2, Point orig,
                int count = 100, double xscale = 25, double yscale = 25);

  Flex_Function(double (*f)(double), double r1, double r2, Point orig,
                int count = 100, double xscale = 25, double yscale = 25);

  void add(Point p) { points.push_back(p); }
  void draw_lines() const override;
  int number_of_points() const { return points.size(); }

  void change_range(double d1, double d2);
  void change_function(Fct f);
  void change_function(double (*f)(double));

private:
  Fct fnct;
  double x1, x2, xs, ys;
  Point center;
  int n;
  vector<Point> points;
  void reset();
};

//-----------------------------------------------------------------------------

Flex_Function::Flex_Function(Fct f, double r1, double r2, Point xy,
                             int count, double xscale, double yscale)
    : fnct{f}, x1{r1}, x2{r2}, center{xy}, n{count}, xs{xscale}, ys{yscale}
// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
// x coordinates are scaled by xscale and y coordinates scaled by yscale
{
  reset();
}

Flex_Function::Flex_Function(double (*f)(double), double r1, double r2, Point xy,
                             int count, double xscale, double yscale)
    : Flex_Function{static_cast<Fct>(f), r1, r2, xy, count, xscale, yscale}
{
}

void Flex_Function::reset()
{
  if (x2 - x1 <= 0)
    error("bad graphing range");
  if (n <= 0)
    error("non-positive graphing count");

  points.clear();
  double dist = (x2 - x1) / n;
  double x = x1;
  for (int i = 0; i < n; ++i)
  {
    add(Point{center.x + int(x * xs), center.y - int(fnct(x) * ys)});
    x += dist;
  }
}

void Flex_Function::change_range(double d1, double d2)
{
  x1 = d1;
  x2 = d2;
  reset();
}

void Flex_Function::change_function(Fct f)
{
  fnct = f;
  reset();
}

void Flex_Function::change_function(double (*f)(double))
{
  fnct = f;
  reset();
}

void Flex_Function::draw_lines() const
{
  if (fill_color().visibility())
  {
    fl_color(fill_color().as_int());
    fl_begin_complex_polygon();
    for (int i = 0; i < number_of_points(); ++i)
    {
      fl_vertex(point(i).x, point(i).y);
    }
    fl_end_complex_polygon();
    fl_color(color().as_int()); // reset color
  }

  if (color().visibility() && 1 < points.size()) // draw sole pixel?
    for (unsigned int i = 1; i < points.size(); ++i)
      fl_line(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
}

//------------------------------------------------------------------------------------

constexpr int xmax = 700;
constexpr int ymax = 700;

const string sinus = "sin(x)",
             cosine = "cos(x)",
             exponent = "exp(x)",
             logarithm = "ln(x)",
             quadratic = "x^2",
             cubic = "x^3",
             sqroot = "sqrt(x)";

//------------------------------------------------------------------------------------

double zero(double x) { return 0; }

double square(double x) { return x * x; }

double cube(double x) { return pow(x, 3); }

//------------------------------------------------------------------------------------

struct My_window : Graph_lib::Window
{
  My_window(Point xy, int w, int h, const string &title, int r1 = -5, int r2 = 5)
      : Window(xy, w, h, title),
        r_min{r1},
        r_max{r2},
        xlength{w - 200},
        ylength{h - 200},
        menu{Point{0, 0}, 70, 20, Menu::vertical, "functions"},
        inx1{Point{150, 0}, 50, 20, "x from:"},
        inx2{Point{250, 0}, 50, 20, "x to:"},
        inxtext{Point{320, 14}, "from " + to_string(r_min) + " to " + to_string(r_max) + " by default"},
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit}
  {
    attach(x);
    attach(y);
    x.set_color(Color::red);
    y.set_color(Color::red);
    x.label.move(xlength / 2, 0);
    menu.attach(new Button{Point{0, 0}, 0, 0, sinus, cb_sin});
    menu.attach(new Button{Point{0, 0}, 0, 0, cosine, cb_cos});
    menu.attach(new Button{Point{0, 0}, 0, 0, exponent, cb_exp});
    menu.attach(new Button{Point{0, 0}, 0, 0, logarithm, cb_log});
    menu.attach(new Button{Point{0, 0}, 0, 0, quadratic, cb_square});
    menu.attach(new Button{Point{0, 0}, 0, 0, cubic, cb_cube});
    menu.attach(new Button{Point{0, 0}, 0, 0, sqroot, cb_sqroot});
    attach(inx1);
    attach(inx2);
    attach(inxtext);
    attach(f);
    f.set_style(Line_style(Line_style::solid, 2));
    f.set_color(Color(Color::invisible));
    attach(menu);
    attach(quit_button);
  }

private:
  const int r_min, r_max;
  const int xlength, ylength;
  const int n_points = xlength;
  const int x_scale = xlength / (r_max - r_min);
  const int y_scale = x_scale;
  const int x_orig = xmax / 2;
  const int y_orig = ymax / 2;
  const Point orig{x_orig, y_orig};
  string xpix = to_string(x_scale), ypix = to_string(y_scale);
  Axis x{Axis::x, Point{100, y_orig}, xlength, xlength / x_scale, "1=" + xpix + "px"};
  Axis y{Axis::y, Point{x_orig, ymax - 100}, ylength, ylength / y_scale, "1=" + ypix + "px"};
  Menu menu;
  In_box inx1, inx2;
  Text inxtext;
  Button quit_button;
  Flex_Function f{zero, r_min, r_max, orig, n_points, x_scale, y_scale};
  static void cb_sin(Address, Address pw) { reference_to<My_window>(pw).set_function(sinus); }
  static void cb_cos(Address, Address pw) { reference_to<My_window>(pw).set_function(cosine); }
  static void cb_exp(Address, Address pw) { reference_to<My_window>(pw).set_function(exponent); }
  static void cb_log(Address, Address pw) { reference_to<My_window>(pw).set_function(logarithm); }
  static void cb_square(Address, Address pw) { reference_to<My_window>(pw).set_function(quadratic); }
  static void cb_cube(Address, Address pw) { reference_to<My_window>(pw).set_function(cubic); }
  static void cb_sqroot(Address, Address pw) { reference_to<My_window>(pw).set_function(sqroot); }
  static void cb_quit(Address, Address pw) { reference_to<My_window>(pw).quit(); }
  void set_range();
  void set_function(const string &s);
  void quit() { Window::hide(); }
};

//-----------------------------------------------------------------------------------------

void My_window::set_range()
{
  string strx1 = inx1.get_string(), strx2 = inx2.get_string();
  stringstream ss1{strx1}, ss2{strx2};
  double x1, x2;
  ss1 >> x1;
  ss2 >> x2;
  if (!ss1 || !ss2)
    return;
  f.change_range(x1, x2);
}

void My_window::set_function(const string &s)
{
  set_range();
  if (s == sinus)
    f.change_function(sin);
  else if (s == cosine)
    f.change_function(cos);
  else if (s == exponent)
    f.change_function(exp);
  else if (s == logarithm)
    f.change_function(log);
  else if (s == quadratic)
    f.change_function(square);
  else if (s == cubic)
    f.change_function(cube);
  else if (s == sqroot)
    f.change_function(sqrt);
  f.set_color(Color(Color::visible));
  redraw();
}

//-----------------------------------------------------------------------------------------

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_window win{top_left, xmax, ymax, "Functions"};
  return gui_main();
}
catch (const exception &e)
{
  cerr << "Exception: " << e.what() << '\n';
  return 1;
}
catch (...)
{
  cerr << "Some kind of exception\n";
  return 2;
}