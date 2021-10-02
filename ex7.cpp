#include "Simple_window.h"
#include "Graph.h"

struct My_airplane : Graph_lib::Window
{
  My_airplane(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<My_airplane>(pw).quit();
                    }}
  {
    attach(quit_button);
    attach(plane);
  }

  void go();

private:
  Button quit_button;
  Image plane{Point{-500, 0}, "image.jpg"};
  void quit() { Window::hide(); }
};

void My_airplane::go()
{
  plane.move(1, 0);
  redraw();
  if (plane.point(0).x > x_max())
    plane.move(-x_max() - 500, 1);

}

//-------------------------------------------------------------------------------------------

Point top_left{0, 0}; //will be top left corner of window
My_airplane win{top_left, x_max(), y_max(), "My airplane"};

//-------------------------------------------------------------------------------------------

void callback(void *)
{
  win.go();
  Fl::repeat_timeout(0.01, callback); // retrigger timeout
}

//-------------------------------------------------------------------------------------------

int main()
try
{
  Fl::add_timeout(0, callback); // set up first timeout
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