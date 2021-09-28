#include "Simple_window.h"
#include "Graph.h"

struct My_window : Graph_lib::Window
{
  My_window(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        next_button{Point{x_max() - 140, 0}, 70, 20, "Next",
                    [](Address, Address pw)
                    {
                      reference_to<My_window>(pw).next();
                    }},
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<My_window>(pw).quit();
                    }}
  {
    attach(next_button);
    attach(quit_button);
  }

private:
  Button next_button;
  Button quit_button;
  void next() {}
  void quit() { Window::hide(); }
};

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_window win{top_left, 800, 400, "lines"};
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