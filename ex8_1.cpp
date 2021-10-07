#include "Simple_window.h"
#include "Graph.h"
#include <iomanip> // std::setprecision

struct My_window : Graph_lib::Window
{
  My_window(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        inbox{Point{70, 0}, 90, 20, "Amount:"},
        outbox{Point{500, 0}, 90, 20, ""},
        menu1_button{Point{250, 0}, 40, 20, "?",
                     [](Address, Address pw)
                     {
                       reference_to<My_window>(pw).menu_from();
                     }},
        menu2_button{Point{330, 0}, 40, 20, "?",
                     [](Address, Address pw)
                     {
                       reference_to<My_window>(pw).menu_to();
                     }},
        available{Point{250, 0}, 40, 20, Menu::vertical, "From"},
        desired{Point{330, 0}, 40, 20, Menu::vertical, "To"},
        calculate_button{Point{400, 0}, 100, 20, "Calculate",
                         [](Address, Address pw)
                         {
                           reference_to<My_window>(pw).calculate();
                         }},
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<My_window>(pw).quit();
                    }}
  {
    attach(inbox);
    attach(from);
    attach(to);
    attach(outbox);
    available.attach(new Button{Point{0, 0}, 0, 0, "USD", [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_usd_pressed();
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, "EUR", [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_eur_pressed();
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, "GBP", [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_gbp_pressed();
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, "CNY", [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_cny_pressed();
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, "UAH", [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_uah_pressed();
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, "RUB", [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_rub_pressed();
                                }});
    //-----------------------------------------------------------------------------------------------
    desired.attach(new Button{Point{0, 0}, 0, 0, "UAH", [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_uah_pressed();
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, "USD", [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_usd_pressed();
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, "EUR", [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_eur_pressed();
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, "GBP", [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_gbp_pressed();
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, "CNY", [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_cny_pressed();
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, "RUB", [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_rub_pressed();
                              }});
    attach(available);
    available.hide();
    attach(desired);
    desired.hide();
    attach(menu1_button);
    attach(menu2_button);
    attach(calculate_button);
    attach(quit_button);
  }

private:
  In_box inbox;
  Text from{Point{200, 14}, "From:"}, to{Point{300, 14}, "To:"};
  Out_box outbox;
  Menu available, desired;
  Button menu1_button, menu2_button, calculate_button, quit_button;

  double usd_uah = 26.6161, uah_usd = 1 / usd_uah;
  double usd_eur = 0.861171, eur_usd = 1 / usd_eur;
  double usd_gbp = 0.737813, gbp_usd = 1 / usd_gbp;
  double usd_cny = 6.44631, cny_usd = 1 / usd_cny;
  double usd_rub = 72.8871, rub_usd = 1 / usd_rub;

  double eur_uah = 30.8272, uah_eur = 1 / eur_uah;
  double eur_gbp = 0.855086, gbp_eur = 1 / eur_gbp;
  double eur_cny = 7.49023, cny_eur = 1 / eur_cny;
  double eur_rub = 84.7545, rub_eur = 1 / eur_rub;

  double gbp_uah = 36.0892, uah_gbp = 1 / gbp_uah;
  double gbp_cny = 8.76700, cny_gbp = 1 / gbp_cny;
  double gbp_rub = 99.1629, rub_gbp = 1 / gbp_rub;

  double cny_uah = 4.11579, uah_cny = 1 / cny_uah;
  double cny_rub = 11.2863, rub_cny = 1 / cny_rub;

  double uah_rub = 2.73861, rub_uah = 1 / cny_uah;

  bool i_usd, i_eur, i_gbp, i_cny, i_uah, i_rub;
  bool o_uah, o_eur, o_gbp, o_cny, o_rub, o_usd;
  void i_usd_pressed();
  void i_eur_pressed();
  void i_gbp_pressed();
  void i_cny_pressed();
  void i_uah_pressed();
  void i_rub_pressed();
  void o_usd_pressed();
  void o_uah_pressed();
  void o_eur_pressed();
  void o_gbp_pressed();
  void o_cny_pressed();
  void o_rub_pressed();
  double get_ex_rate();
  void calculate();
  void menu_from();
  void menu_to();
  void hide_menu1();
  void hide_menu2();
  void quit() { Window::hide(); }
};

void My_window::hide_menu1()
{
  available.hide();
  menu1_button.show();
}

void My_window::hide_menu2()
{
  desired.hide();
  menu2_button.show();
}

void My_window::menu_from()
{
  menu1_button.hide();
  available.show();
}

void My_window::menu_to()
{
  menu2_button.hide();
  desired.show();
}

void My_window::i_usd_pressed()
{
  i_usd = true;
  i_eur = false;
  i_gbp = false;
  i_cny = false;
  i_uah = false;
  i_rub = false;
  hide_menu1();
  menu1_button.label = "USD";
}

void My_window::i_eur_pressed()
{
  i_eur = true;
  i_usd = false;
  i_gbp = false;
  i_cny = false;
  i_uah = false;
  i_rub = false;
  hide_menu1();
  menu1_button.label = "EUR";
}

void My_window::i_gbp_pressed()
{
  i_gbp = true;
  i_usd = false;
  i_eur = false;
  i_cny = false;
  i_uah = false;
  i_rub = false;
  hide_menu1();
  menu1_button.label = "GBP";
}

void My_window::i_cny_pressed()
{
  i_cny = true;
  i_gbp = false;
  i_usd = false;
  i_eur = false;
  i_uah = false;
  i_rub = false;
  hide_menu1();
  menu1_button.label = "CNY";
}

void My_window::i_uah_pressed()
{
  i_uah = true;
  i_cny = false;
  i_gbp = false;
  i_usd = false;
  i_eur = false;
  i_rub = false;
  hide_menu1();
  menu1_button.label = "UAH";
}

void My_window::i_rub_pressed()
{
  i_rub = true;
  i_uah = false;
  i_cny = false;
  i_gbp = false;
  i_usd = false;
  i_eur = false;
  hide_menu1();
  menu1_button.label = "RUB";
}

void My_window::o_usd_pressed()
{
  o_usd = true;
  o_uah = false;
  o_eur = false;
  o_gbp = false;
  o_cny = false;
  o_rub = false;
  hide_menu2();
  menu2_button.label = "USD";
}

void My_window::o_uah_pressed()
{
  o_uah = true;
  o_eur = false;
  o_gbp = false;
  o_cny = false;
  o_rub = false;
  o_usd = false;
  hide_menu2();
  menu2_button.label = "UAH";
}

void My_window::o_eur_pressed()
{
  o_eur = true;
  o_uah = false;
  o_gbp = false;
  o_cny = false;
  o_rub = false;
  o_usd = false;
  hide_menu2();
  menu2_button.label = "EUR";
}

void My_window::o_gbp_pressed()
{
  o_gbp = true;
  o_eur = false;
  o_uah = false;
  o_cny = false;
  o_rub = false;
  o_usd = false;
  hide_menu2();
  menu2_button.label = "GBP";
}

void My_window::o_cny_pressed()
{
  o_cny = true;
  o_gbp = false;
  o_eur = false;
  o_uah = false;
  o_rub = false;
  o_usd = false;
  hide_menu2();
  menu2_button.label = "CNY";
}

void My_window::o_rub_pressed()
{
  o_rub = true;
  o_cny = false;
  o_gbp = false;
  o_eur = false;
  o_uah = false;
  o_usd = false;
  hide_menu2();
  menu2_button.label = "RUB";
}

//------------------------------------------------------------------------------------------

double My_window::get_ex_rate()
{
  if (i_usd)
  {
    if (o_uah)
      return usd_uah;
    else if (o_eur)
      return usd_eur;
    else if (o_gbp)
      return usd_gbp;
    else if (o_cny)
      return usd_cny;
    else if (o_rub)
      return usd_rub;
  }
  else if (i_eur)
  {
    if (o_uah)
      return eur_uah;
    else if (o_usd)
      return eur_usd;
    else if (o_gbp)
      return eur_gbp;
    else if (o_cny)
      return eur_cny;
    else if (o_rub)
      return eur_rub;
  }
  else if (i_gbp)
  {
    if (o_uah)
      return gbp_uah;
    else if (o_usd)
      return gbp_usd;
    else if (o_eur)
      return gbp_eur;
    else if (o_cny)
      return gbp_cny;
    else if (o_rub)
      return gbp_rub;
  }
  else if (i_cny)
  {
    if (o_uah)
      return cny_uah;
    else if (o_usd)
      return cny_usd;
    else if (o_eur)
      return cny_eur;
    else if (o_gbp)
      return cny_gbp;
    else if (o_rub)
      return cny_rub;
  }
  else if (i_uah)
  {
    if (o_rub)
      return uah_rub;
    else if (o_usd)
      return uah_usd;
    else if (o_eur)
      return uah_eur;
    else if (o_gbp)
      return uah_gbp;
    else if (o_rub)
      return uah_rub;
  }
  else if (i_rub)
  {
    if (o_uah)
      return rub_uah;
    else if (o_usd)
      return rub_usd;
    else if (o_eur)
      return rub_eur;
    else if (o_gbp)
      return rub_gbp;
    else if (o_cny)
      return rub_cny;
  }
  return 1; //if currencies are the same
}

void My_window::calculate()
{
  string s = inbox.get_string();
  istringstream is{s};
  ostringstream os;
  double ihave, iget, ex_rate = get_ex_rate();
  is >> ihave;
  iget = ihave * ex_rate;
  os << fixed << setprecision(2) << iget;
  outbox.put(os.str());
  redraw();
}

//-----------------------------------------------------------------------------------------------

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_window win{top_left, 800, 400, "Currency converter"};
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