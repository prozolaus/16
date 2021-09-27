#include "Simple_window.h"
#include "Graph.h"

struct Regular_polygon : Closed_polyline
{
  Regular_polygon(Point center, int count_of_sides, int radius);
  Point center() { return c; }

  void move(int dx, int dy) override
  {
    Closed_polyline::move(dx, dy);
    c.x += dx;
    c.y += dy;
  }

private:
  Point c;
  int count;
  int R;
  Point coords(const double &angle);
};

Regular_polygon::Regular_polygon(Point center, int count_of_sides, int radius)
    : c{center}, count{count_of_sides}, R{radius}
{
  if (count_of_sides < 3)
    error("The count of sides is less than three!");
  int step = round(360 / count);
  for (double i = 0; i < 360; i += step)
    add(coords(i));
}

Point Regular_polygon::coords(const double &angle)
{
  int x = round(R * cos(angle * M_PI / 180));
  int y = round(R * sin(angle * M_PI / 180));
  return Point{c.x + x, c.y - y};

}
//------------------------------------------------------------------------------------

struct My_window : Graph_lib::Window
{
  My_window(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        in_x{Point{20, 0}, 50, 20, "x:"},
        in_y{Point{100, 0}, 50, 20, "y:"},
        circle_button{Point{0, 30}, 70, 20, "Circle",
                      [](Address, Address pw)
                      {
                        reference_to<My_window>(pw).draw_circle();
                      }},
        square_button{Point{0, 50}, 70, 20, "Square",
                      [](Address, Address pw)
                      {
                        reference_to<My_window>(pw).draw_square();
                      }},
        triangle_button{Point{0, 70}, 70, 20, "Triangle",
                        [](Address, Address pw)
                        {
                          reference_to<My_window>(pw).draw_triangle();
                        }},
        reg_hex_button{Point{0, 90}, 70, 20, "Reg Hex",
                       [](Address, Address pw)
                       {
                         reference_to<My_window>(pw).draw_reg_hex();
                       }},
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<My_window>(pw).quit();
                    }}
  {
    attach(in_x);
    attach(in_y);
    attach(circle_button);
    attach(c);
    c.set_color(Color::invisible);
    attach(square_button);
    attach(r);
    r.set_color(Color::invisible);
    attach(triangle_button);
    attach(rtr);
    rtr.set_color(Color::invisible);
    attach(reg_hex_button);
    attach(rh);
    rh.set_color(Color::invisible);
    attach(quit_button);
  }

private:
  In_box in_x, in_y;
  Button circle_button;
  Button square_button;
  Button triangle_button;
  Button reg_hex_button;
  Button quit_button;
  Circle c{Point{0, 0}, 20};
  Rectangle r{Point{0, 0}, 40, 40};
  Regular_polygon rtr{Point{0, 0}, 3, 20};
  Regular_polygon rh{Point{0, 0}, 6, 20};
  void draw_circle();
  void draw_square();
  void draw_triangle();
  void draw_reg_hex();
  void quit() { Window::hide(); }
};

void My_window::draw_circle()
{
  int x = in_x.get_int();
  int y = in_y.get_int();
  Point cur_c = c.center();
  c.move(-cur_c.x + x, -cur_c.y + y);
  c.set_color(Color::visible);
  redraw();
}

void My_window::draw_square()
{
  int x = in_x.get_int();
  int y = in_y.get_int();
  Point cur_c = r.point(0);
  r.move(-cur_c.x + x - r.width() / 2, -cur_c.y + y - r.height() / 2);
  r.set_color(Color::visible);
  redraw();
}

void My_window::draw_triangle()
{
  int x = in_x.get_int();
  int y = in_y.get_int();
  Point cur_c = rtr.center();
  rtr.move(-cur_c.x + x, -cur_c.y + y);
  rtr.set_color(Color::visible);
  redraw();
}

void My_window::draw_reg_hex()
{
  int x = in_x.get_int();
  int y = in_y.get_int();
  Point cur_c = rh.center();
  rh.move(-cur_c.x + x, -cur_c.y + y);
  rh.set_color(Color::visible);
  redraw();
}

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_window win{top_left, 800, 400, "ex4"};
  return gui_main();
}
catch (const std::exception &e)
{
  std::cerr << "Exception: " << e.what() << '\n';
  return 1;
}
catch (...)
{
  cerr << "Some kind of exception\n";
  return 2;
}