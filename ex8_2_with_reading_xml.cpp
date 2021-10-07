#include "Simple_window.h"
#include "Graph.h"
#include <iomanip> // std::setprecision

const string uah = "UAH",
             usd = "USD",
             eur = "EUR",
             gbp = "GBP",
             cny = "CNY",
             rub = "RUB",
             pln = "PLN";

struct Data
{
  Data(string name, double ex_rate)
      : code{name}, rate{ex_rate}
  {
  }
  string code;
  double rate;
};

struct My_window : Graph_lib::Window
{
  My_window(Point xy, int w, int h, const string &title, const string &xml_file)
      : Window(xy, w, h, title),
        width{w},
        height{h},
        filename{xml_file},
        inbox{Point{(int)width / 10, 0}, 90, 20, "Amount:"},
        outbox{Point{int(width / 1.4), 0}, 90, 20, ""},
        menu1_button{Point{int(width / 2.8), 0}, 40, 20, "?",
                     [](Address, Address pw)
                     {
                       reference_to<My_window>(pw).menu_from();
                     }},
        menu2_button{Point{int(width / 2.12), 0}, 40, 20, "?",
                     [](Address, Address pw)
                     {
                       reference_to<My_window>(pw).menu_to();
                     }},
        available{Point{int(width / 2.8), 0}, 40, 20, Menu::vertical, "From"},
        desired{Point{int(width / 2.12), 0}, 40, 20, Menu::vertical, "To"},
        calculate_button{Point{int(width / 1.75), 0}, 100, 20, "Calculate",
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
    available.attach(new Button{Point{0, 0}, 0, 0, usd, [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_pressed(usd);
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, eur, [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_pressed(eur);
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, gbp, [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_pressed(gbp);
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, cny, [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_pressed(cny);
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, uah, [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_pressed(uah);
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, rub, [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_pressed(rub);
                                }});
    available.attach(new Button{Point{0, 0}, 0, 0, pln, [](Address, Address pw)
                                {
                                  reference_to<My_window>(pw).i_pressed(pln);
                                }});
    //-----------------------------------------------------------------------------------------------
    desired.attach(new Button{Point{0, 0}, 0, 0, uah, [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_pressed(uah);
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, usd, [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_pressed(usd);
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, eur, [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_pressed(eur);
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, gbp, [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_pressed(gbp);
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, cny, [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_pressed(cny);
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, rub, [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_pressed(rub);
                              }});
    desired.attach(new Button{Point{0, 0}, 0, 0, pln, [](Address, Address pw)
                              {
                                reference_to<My_window>(pw).o_pressed(pln);
                              }});
    check_dimensions();
    attach(available);
    available.hide();
    attach(desired);
    desired.hide();
    attach(menu1_button);
    attach(menu2_button);
    attach(calculate_button);
    attach(quit_button);
    read_file();
  }

private:
  int width, height;
  In_box inbox;
  Text from{Point{int(width / 3.5), 14}, "From:"}, to{Point{int(width / 2.33), 14}, "To:"};
  Out_box outbox;
  Menu available, desired;
  Button menu1_button, menu2_button, calculate_button, quit_button;
  string filename;
  Vector_ref<Data> vd;

  bool i_usd, i_eur, i_gbp, i_cny, i_uah, i_rub, i_pln;
  bool o_uah, o_eur, o_gbp, o_cny, o_rub, o_usd, o_pln;
  void check_dimensions();
  void read_file();
  void i_pressed(const string &str);
  void o_pressed(const string &str);
  double get_ex_rate();
  double search_ex_rate(const string &str);
  void calculate();
  void menu_from();
  void menu_to();
  void hide_menu1();
  void hide_menu2();
  void quit() { Window::hide(); }
};

void My_window::check_dimensions()
{
  if (width < 700)
    error("Width of the window is too small! We recommend 700 px.");
  if (height < 150)
    error("Height of the window is too small! We recommend 150 px.");
}

void My_window::read_file()
//we expect this format of a xml-file:
//<rate>19.125</rate>
//<cc>AUD</cc>
{
  ifstream ifs{filename};
  if (!ifs)
    error("Can not read file ", filename);
  const string ratestr1 = "<rate>", ratestr2 = "</rate>", ccstr1 = "<cc>", ccstr2 = "</cc>";
  string s;
  double d;
  const int n = 2; //we need two parameters
  while (ifs)
  {
    for (int i = 0, index1, index2, pos, len; i < n; i++)
    {
      string s1 = (!i) ? ratestr1 : ccstr1, s2 = (!i) ? ratestr2 : ccstr2;
      ifs >> s;
      index1 = s.find(s1), index2 = s.find(s2);
      if (index1 > -1 && index2 > -1)
      {
        pos = index1 + s1.size(), len = index2 - pos;
        s = s.substr(pos, len);
        if (!i)
        {
          istringstream is{s};
          is >> d;
          if (!is)
            error("Double search error!");
          if (d == 0)
            error("Сurrency rate cannot be zero");
        }
        else
        {
          if (s.size() != 3)
            error("Wrong format of currency code!");
          vd.push_back(new Data(s, d));
        }
      }
      else
        break;
    }
  }
  if (vd.size() == 0)
    error("Wrong data in file ", filename);
}

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
  if (!(o_uah && !i_uah))
  {
    menu2_button.hide();
    desired.show();
  }
}

void My_window::i_pressed(const string &str)
{
  i_usd = false;
  i_eur = false;
  i_gbp = false;
  i_cny = false;
  i_uah = false;
  i_rub = false;
  i_pln = false;
  if (str == usd)
    i_usd = true;
  else if (str == eur)
    i_eur = true;
  else if (str == gbp)
    i_gbp = true;
  else if (str == cny)
    i_cny = true;
  else if (str == uah)
    i_uah = true;
  else if (str == rub)
    i_rub = true;
  else if (str == pln)
    i_pln = true;
  hide_menu1();
  menu1_button.label = str;
  if (str != uah)
    o_pressed(uah);
}

void My_window::o_pressed(const string &str)
{
  o_usd = false;
  o_uah = false;
  o_eur = false;
  o_gbp = false;
  o_cny = false;
  o_rub = false;
  o_pln = false;
  if (str == usd)
    o_usd = true;
  else if (str == eur)
    o_eur = true;
  else if (str == gbp)
    o_gbp = true;
  else if (str == cny)
    o_cny = true;
  else if (str == uah)
    o_uah = true;
  else if (str == rub)
    o_rub = true;
  else if (str == pln)
    o_pln = true;
  hide_menu2();
  menu2_button.label = str;
}

//------------------------------------------------------------------------------------------

double My_window::search_ex_rate(const string &str)
{
  for (int i = 0; i < vd.size(); i++)
    if (vd[i].code == str)
      return vd[i].rate;
  return 1;
}

double My_window::get_ex_rate()
{
  double d = 1;
  if (i_uah)
  {
    if (o_rub)
      d = 1 / search_ex_rate(rub);
    else if (o_usd)
      d = 1 / search_ex_rate(usd);
    else if (o_eur)
      d = 1 / search_ex_rate(eur);
    else if (o_gbp)
      d = 1 / search_ex_rate(gbp);
    else if (o_cny)
      d = 1 / search_ex_rate(cny);
    else if (o_pln)
      d = 1 / search_ex_rate(pln);
  }
  else if (o_uah)
  {
    if (i_usd)
      d = search_ex_rate(usd);
    else if (i_eur)
      d = search_ex_rate(eur);
    else if (i_gbp)
      d = search_ex_rate(gbp);
    else if (i_cny)
      d = search_ex_rate(cny);
    else if (i_rub)
      d = search_ex_rate(rub);
    else if (i_pln)
      d = search_ex_rate(pln);
  }

  return d; //if currencies are the same
}

void My_window::calculate()
{
  string s = inbox.get_string();
  istringstream is{s};
  ostringstream os;
  double ihave = 0, iget = 0, ex_rate = get_ex_rate();
  is >> ihave;
  iget = ihave * ex_rate;
  os << fixed << setprecision(2) << iget;
  outbox.put(os.str());
  redraw();
}

//-----------------------------------------------------------------------------------------------

int main()
//please, run file called "script_ex8" to get updated file "ex_rates.xml"
// this script-file has two lines:
//#!/bin/bash
//wget -O ex_rates.xml https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_window win{top_left, 700, 150, "Currency converter for Ukraine", "ex_rates.xml"};
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