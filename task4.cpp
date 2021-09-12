#include "Simple_window.h"
#include "Graph.h"

struct Lines_window : Graph_lib::Window
{
  Lines_window(Point xy, int w, int h, const string &title);

private:
  Open_polyline lines;

  Button next_button;
  Button quit_button;
  In_box next_x;
  In_box next_y;
  Out_box xy_out;
  Menu color_menu;
  Button color_button;
  Menu style_menu;
  Button style_button;

  void change_color(Color c) { lines.set_color(c); }
  void change_style(Line_style ls) { lines.set_style(ls); }

  void hide_color_menu()
  {
    color_menu.hide();
    color_button.show();
  }

  void hide_style_menu()
  {
    style_menu.hide();
    style_button.show();
  }

  static void cb_red(Address, Address pw) { reference_to<Lines_window>(pw).red_pressed(); }
  static void cb_blue(Address, Address pw) { reference_to<Lines_window>(pw).blue_pressed(); }
  static void cb_black(Address, Address pw) { reference_to<Lines_window>(pw).black_pressed(); }
  static void cb_next(Address, Address pw) { reference_to<Lines_window>(pw).next(); }
  static void cb_quit(Address, Address pw) { reference_to<Lines_window>(pw).quit(); }
  static void cb_menu(Address, Address pw) { reference_to<Lines_window>(pw).menu_pressed(); }
  static void cb_style(Address, Address pw) { reference_to<Lines_window>(pw).style_pressed(); }
  static void cb_dotted(Address, Address pw) { reference_to<Lines_window>(pw).dotted_pressed(); }
  static void cb_dashed(Address, Address pw) { reference_to<Lines_window>(pw).dashed_pressed(); }
  static void cb_solid(Address, Address pw) { reference_to<Lines_window>(pw).solid_pressed(); }

  void red_pressed()
  {
    change_color(Color::red);
    hide_color_menu();
  }

  void blue_pressed()
  {
    change_color(Color::blue);
    hide_color_menu();
  }

  void black_pressed()
  {
    change_color(Color::black);
    hide_color_menu();
  }

  void dotted_pressed()
  {
    change_style(Line_style(Line_style::dot));
    hide_style_menu();
  }

  void dashed_pressed()
  {
    change_style(Line_style(Line_style::dash));
    hide_style_menu();
  }

  void solid_pressed()
  {
    change_style(Line_style(Line_style::solid));
    hide_style_menu();
  }

  void menu_pressed()
  {
    color_button.hide();
    color_menu.show();
  }

  void style_pressed()
  {
    style_button.hide();
    style_menu.show();
  }

  void next();
  void quit();
};

Lines_window::Lines_window(Point xy, int w, int h, const string &title)
    : Window{xy, w, h, title},
      next_button{Point{x_max() - 250, 0}, 80, 20, "Next point", cb_next},
      quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit", cb_quit},
      next_x{Point{x_max() - 410, 0}, 50, 20, "next x:"},
      next_y{Point{x_max() - 310, 0}, 50, 20, "next y:"},
      xy_out{Point{100, 0}, 100, 20, "current (x,y):"},
      color_menu{Point{x_max() - 70, 40}, 70, 20, Menu::vertical, "color"},
      color_button{Point{x_max() - 90, 30}, 90, 20, "color menu", cb_menu},
      style_menu{Point{x_max() - 170, 40}, 70, 20, Menu::vertical, "style"},
      style_button{Point{x_max() - 190, 30}, 90, 20, "style menu", cb_style}
{
  attach(next_button);
  attach(quit_button);
  attach(next_x);
  attach(next_y);
  attach(xy_out);
  xy_out.put("no point");
  color_menu.attach(new Button{Point{0, 0}, 0, 0, "red", cb_red});
  color_menu.attach(new Button{Point{0, 0}, 0, 0, "blue", cb_blue});
  color_menu.attach(new Button{Point{0, 0}, 0, 0, "black", cb_black});
  attach(color_menu);
  color_menu.hide();
  attach(color_button);
  style_menu.attach(new Button{Point{0, 0}, 0, 0, "dotted", cb_dotted});
  style_menu.attach(new Button{Point{0, 0}, 0, 0, "dashed", cb_dashed});
  style_menu.attach(new Button{Point{0, 0}, 0, 0, "solid", cb_solid});
  attach(style_menu);
  style_menu.hide();
  attach(style_button);
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