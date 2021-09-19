#include "Simple_window.h"
#include "Graph.h"

struct My_window : Graph_lib::Window
{
  My_window(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        ob{Point{100, 0}, 70, 20, "loc: "},
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<My_window>(pw).quit();
                    }}
  {
    const int bw = 59, bh = 59;
    int i = 0;
    vb.push_back(new Button{Point{60, 60}, bw, bh, to_string(++i), cb_next1});
    vb.push_back(new Button{Point{120, 60}, bw, bh, to_string(++i), cb_next2});
    vb.push_back(new Button{Point{180, 60}, bw, bh, to_string(++i), cb_next3});
    vb.push_back(new Button{Point{240, 60}, bw, bh, to_string(++i), cb_next4});
    vb.push_back(new Button{Point{60, 120}, bw, bh, to_string(++i), cb_next5});
    vb.push_back(new Button{Point{120, 120}, bw, bh, to_string(++i), cb_next6});
    vb.push_back(new Button{Point{180, 120}, bw, bh, to_string(++i), cb_next7});
    vb.push_back(new Button{Point{240, 120}, bw, bh, to_string(++i), cb_next8});
    vb.push_back(new Button{Point{60, 180}, bw, bh, to_string(++i), cb_next9});
    vb.push_back(new Button{Point{120, 180}, bw, bh, to_string(++i), cb_next10});
    vb.push_back(new Button{Point{180, 180}, bw, bh, to_string(++i), cb_next11});
    vb.push_back(new Button{Point{240, 180}, bw, bh, to_string(++i), cb_next12});
    vb.push_back(new Button{Point{60, 240}, bw, bh, to_string(++i), cb_next13});
    vb.push_back(new Button{Point{120, 240}, bw, bh, to_string(++i), cb_next14});
    vb.push_back(new Button{Point{180, 240}, bw, bh, to_string(++i), cb_next15});
    vb.push_back(new Button{Point{240, 240}, bw, bh, to_string(++i), cb_next16});
    for (int i = 0; i < 16; i++)
      attach(vb[i]);
    attach(ob);
    attach(quit_button);
  }

private:
  Out_box ob;
  Vector_ref<Button> vb;

  Button quit_button;
  void quit() { Window::hide(); }

  void do_next(int i) 
  { 
    for (int i = 0; i < 16; i++)
      vb[i].label = to_string(i+1);
    redraw();
    ob.put(to_string(vb[i].loc.x) + ", " + to_string(vb[i].loc.y)); 
    vb[i].label = "clicked"; 
  }

  void next1() { do_next(0); }
  void next2() { do_next(1); }
  void next3() { do_next(2); }
  void next4() { do_next(3); }
  void next5() { do_next(4); }
  void next6() { do_next(5); }
  void next7() { do_next(6); }
  void next8() { do_next(7); }
  void next9() { do_next(8); }
  void next10() { do_next(9); }
  void next11() { do_next(10); }
  void next12() { do_next(11); }
  void next13() { do_next(12); }
  void next14() { do_next(13); }
  void next15() { do_next(14); }
  void next16() { do_next(15); }

  static void cb_next1(Address, Address pw) { reference_to<My_window>(pw).next1(); }
  static void cb_next2(Address, Address pw) { reference_to<My_window>(pw).next2(); }
  static void cb_next3(Address, Address pw) { reference_to<My_window>(pw).next3(); }
  static void cb_next4(Address, Address pw) { reference_to<My_window>(pw).next4(); }
  static void cb_next5(Address, Address pw) { reference_to<My_window>(pw).next5(); }
  static void cb_next6(Address, Address pw) { reference_to<My_window>(pw).next6(); }
  static void cb_next7(Address, Address pw) { reference_to<My_window>(pw).next7(); }
  static void cb_next8(Address, Address pw) { reference_to<My_window>(pw).next8(); }
  static void cb_next9(Address, Address pw) { reference_to<My_window>(pw).next9(); }
  static void cb_next10(Address, Address pw) { reference_to<My_window>(pw).next10(); }
  static void cb_next11(Address, Address pw) { reference_to<My_window>(pw).next11(); }
  static void cb_next12(Address, Address pw) { reference_to<My_window>(pw).next12(); }
  static void cb_next13(Address, Address pw) { reference_to<My_window>(pw).next13(); }
  static void cb_next14(Address, Address pw) { reference_to<My_window>(pw).next14(); }
  static void cb_next15(Address, Address pw) { reference_to<My_window>(pw).next15(); }
  static void cb_next16(Address, Address pw) { reference_to<My_window>(pw).next16(); }
};

int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  My_window win{top_left, 360, 360, "ex2"};
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