#include "Simple_window.h"
#include "Graph.h"
#include <random>

inline int rand_int(int min, int max)
{
  static default_random_engine ran;
  return uniform_int_distribution<>{min, max}(ran);
}

struct My_window : Graph_lib::Window
{
  My_window(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        image_button{Point{0, 0}, 300, 300, "Image",
                     [](Address, Address pw)
                     {
                       reference_to<My_window>(pw).move_image();
                     }},
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<My_window>(pw).quit();
                    }}
  {
    attach(image_button);
    attach(img);
    attach(quit_button);
    img.set_mask(Point{0, 0}, image_button.width, image_button.height);
  }

private:
  Button image_button;
  Image img{Point{0, 0}, "vegetables.jpg"};
  void move_image();
  Button quit_button;
  void quit() { Window::hide(); }
};

void My_window::move_image()
{
  int dx, dy, new_x, new_y;
  const int width = x_max() - image_button.width, height = y_max() - image_button.height;
  while (true)
  {
    dx = rand_int(-width, width);
    dy = rand_int(-height, height);
    new_x = image_button.loc.x + dx;
    new_y = image_button.loc.y + dy;
    if (new_x >= 0 && new_y >= 0 && new_x <= width && new_y <= height)
      break;
  }
  image_button.move(dx, dy);
  img.move(dx, dy);
  img.set_mask(Point{new_x, new_y}, image_button.width, image_button.height);
}

//-----------------------------------------------------------------------------

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_window win{top_left, x_max(), y_max(), "ex3"};
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