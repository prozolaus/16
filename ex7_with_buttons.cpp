#include "Simple_window.h"
#include "Graph.h"

struct My_airplane : Graph_lib::Window
{
  My_airplane(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        start_button{Point{x_max() - 210, 0}, 70, 20, "Start",
                     [](Address, Address pw)
                     {
                       reference_to<My_airplane>(pw).is_flying = true;
                     }},
        stop_button{Point{x_max() - 140, 0}, 70, 20, "Stop",
                    [](Address, Address pw)
                    {
                      reference_to<My_airplane>(pw).is_flying = false;
                    }},
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<My_airplane>(pw).quit();
                    }}
  {
    attach(start_button);
    attach(stop_button);
    attach(quit_button);
    attach(plane);
  }

  static void cb_fly(void *);
  void fly();

private:
  Button start_button, stop_button, quit_button;
  bool is_flying = false;
  Image plane{Point{0, 0}, "image.jpg"};
  void quit() { Window::hide(); }
};

void My_airplane::cb_fly(void *p)
{
  My_airplane *a = (My_airplane *)p;
  a->fly();
  Fl::repeat_timeout(0.01, cb_fly, a);
}

void My_airplane::fly()
{
  if (is_flying)
  {
    plane.move(1, 0);
    redraw();
  }
  if (plane.point(0).x > x_max())
    plane.move(-x_max() - 500, 1);
}

//-------------------------------------------------------------------------------------------

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_airplane *ma = new My_airplane{top_left, x_max(), y_max(), "My airplane"};
  Fl::add_timeout(1.0, My_airplane::cb_fly, ma);
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