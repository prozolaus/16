#include "Simple_window.h"
#include "Graph.h"

struct My_watch : Graph_lib::Window
{
  My_watch(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<My_watch>(pw).quit();
                    }}
  {
    circle.set_color(0);
    circle.set_style(ls);
    second_hand.set_color(1); //red
    minute_hand.set_color(0);
    minute_hand.set_style(ls);
    hour_hand.set_color(0);
    hour_hand.set_style(ls);
    notches.set_color(0);
    hour_notches.set_color(0);
    hour_notches.set_style(ls);
    attach(circle);
    attach(notches);
    attach(hour_notches);
    attach(hour_hand);
    attach(minute_hand);
    attach(second_hand);
    attach(quit_button);
    init(); //fill point vectors
    get_time_now(); //get system time
  }

  static void callback(void *);
  void go();

private:
  Point center{x_max() / 2, y_max() / 2};
  const int radius = y_max() / 3;
  Circle circle{center, radius};
  Line second_hand{center, Point{center.x, center.y - radius}};
  Line minute_hand{center, Point{center.x, center.y - radius}};
  Line hour_hand{center, Point{center.x, center.y}};
  Lines notches, hour_notches;
  Line_style ls{Line_style::solid, 2};
  const double PI = 3.141592653589793238463;
  double start_angle = 90;
  int sec, min, hour, hour_ampm;                          //first initialization in function get_time_now()
  const int minsec = 60, hrs = 12;                        //60 seconds = 1 minute and 60 minutes = 1 hour, 12 hours = 1 turn
  const int minsec_angel_delta = 6, hrs_angel_delta = 30; //6 degrees = 1 sec(min), 30 degrees = 1 hour
  Button quit_button;
  vector<Point> sec_coords, min_coords, hour_coords, start_coords, notches_coords, hour_notches_coords;
  void init();
  void init_notches();
  void get_time_now();
  void check();
  void get_coords(vector<Point> &vp, double rad, int angel_delta, int n);
  void quit() { Window::hide(); }
};

void My_watch::get_coords(vector<Point> &vp, double rad, const int angel_delta, const int n)
{
  double angle = start_angle;
  for (int i = 0, x, y; i < n; i++)
  {
    x = round(rad * cos(angle * PI / 180));
    y = round(rad * sin(angle * PI / 180));
    vp.push_back(Point{second_hand.point(0).x + x, second_hand.point(0).y - y});
    angle -= angel_delta;
  }
}

void My_watch::init()
{
  double k = 0.95 * radius;
  get_coords(sec_coords, k, minsec_angel_delta, minsec);

  k = 0.9 * radius;
  get_coords(min_coords, k, minsec_angel_delta, minsec);

  k = 0.5 * radius;
  get_coords(hour_coords, k, hrs_angel_delta, hrs);

  k = radius;
  get_coords(start_coords, k, minsec_angel_delta, minsec);

  k = 0.96 * radius;
  get_coords(notches_coords, k, minsec_angel_delta, minsec);

  k = 0.92 * radius;
  get_coords(hour_notches_coords, k, hrs_angel_delta, hrs);

  init_notches();
}

void My_watch::init_notches()
{
  const int every_hour_notch = 5;
  for (int i = 0; i < minsec; i++)
      notches.add(start_coords[i], notches_coords[i]);
  for (int i = 0; i < hrs; i++)
    hour_notches.add(start_coords[i * every_hour_notch], hour_notches_coords[i]);
}

void My_watch::get_time_now()
{
  time_t t = time(0); // get time now
  tm *now = localtime(&t);
  sec = now->tm_sec;
  min = now->tm_min;
  hour = now->tm_hour;
  hour_ampm = hour;
}

void My_watch::check()
{
  if (sec == 60)
  {
    sec = 0;
    min++;
  }
  if (min == 60)
  {
    min = 0;
    hour++;
  }
  if (hour >= 12)
  {
    hour_ampm = hour - 12;
    if (hour == 24)
    {
      hour = 0;
      hour_ampm = 0;
    }
  }
}

void My_watch::go()
{
  check();

  second_hand.change_point(1, sec_coords[sec]);
  minute_hand.change_point(1, min_coords[min]);
  hour_hand.change_point(1, hour_coords[hour_ampm]);

  redraw();
  sec++;
}

void My_watch::callback(void *p)
{
  My_watch *w = (My_watch *)p;
  w->go();
  Fl::repeat_timeout(1.0, callback, w); // retrigger timeout
}

//-------------------------------------------------------------------------------------------

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_watch *mw = new My_watch{top_left, x_max(), y_max(), "My watch"};
  Fl::add_timeout(0, My_watch::callback, mw); // set up first timeout
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