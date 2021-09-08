#include "Simple_window.h"
#include "Graph.h"

struct Lines_window : Graph_lib::Window
{
  Lines_window(Point xy, int w, int h, const string &title);
  Open_polyline lines;

private:
  Button next_button;
  Button quit_button;
  In_box next_x;
  In_box next_y;
  Out_box xy_out;

  void next();
  void quit();
};

Lines_window::Lines_window(Point xy, int w, int h, const string &title)
    : Window{xy, w, h, title},
      next_button{Point{x_max() - 250, 0}, 80, 20, "Next point",
                  [](Address, Address pw)
                  {
                    reference_to<Lines_window>(pw).next();
                  }},
      quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                  [](Address, Address pw)
                  {
                    reference_to<Lines_window>(pw).quit();
                  }},
      next_x{Point{x_max() - 410, 0}, 50, 20, "next x:"},
      next_y{Point{x_max() - 310, 0}, 50, 20, "next y:"},
      xy_out{Point{100, 0}, 100, 20, "current (x,y):"}
{
  attach(next_button);
  attach(quit_button);
  attach(next_x);
  attach(next_y);
  attach(xy_out);
  attach(lines);
}

void Lines_window::quit()
{
  Window::hide();
}

void Lines_window::next()
{
  int x = next_x.get_int();
  int y = next_y.get_int();
  lines.add(Point{x, y});

  ostringstream ss;
  ss << '(' << x << ',' << y << ')';
  xy_out.put(ss.str());

  Window::redraw();
}

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window

  Lines_window win{top_left, 800, 400, "lines"};

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