#include "Simple_window.h"
#include "Graph.h"

struct Regular_polygon : Closed_polyline
{
	Regular_polygon(Point center, int count_of_sides, int radius);
	Point center() { return c; }

	void move(int dx, int dy) override;
	void change_radius(int new_R);

private:
	Point c;
	int count;
	int R;
	Point coords(const double& angle);
};

Regular_polygon::Regular_polygon(Point center, int count_of_sides, int radius)
	: c{ center }, count{ count_of_sides }, R{ radius }
{
	if (count_of_sides < 3)
		error("The count of sides is less than three!");
	int step = round(360 / count);
	double i = 0;
	if (count == 3)	//for triangle
		i = 90; //the first point of the triangle starts at 90 degrees, not zero (optional)
	for (; i < 360; i += step)
		add(coords(i));
}

Point Regular_polygon::coords(const double& angle)
{
	const double PI = 3.141592653589793238463;
	int x = round(R * cos(angle * PI / 180));
	int y = round(R * sin(angle * PI / 180));
	return Point{ c.x + x, c.y - y };
}

void Regular_polygon::change_radius(int new_R)
{
	if (R > 0)
		R = new_R;
	else
		error("Wrong radius!");
	int step = round(360 / count);
	double i = 0;
	if (count == 3)
		i = 90; //the first point of the triangle starts at 90 degrees, not zero (optional)
	for (int cnt = 0; i < 360; i += step, cnt++)
		set_point(cnt, coords(i));
}

void Regular_polygon::move(int dx, int dy)
{
	Closed_polyline::move(dx, dy);
	//center moving
	c.x += dx;
	c.y += dy;
}

//------------------------------------------------------------------------------------------------------

struct My_window : Graph_lib::Window
{
	My_window(Point xy, int w, int h, const string& title)
		: Window(xy, w, h, title),
		in_x{ Point{20, 0}, 50, 20, "x:" },
		in_y{ Point{100, 0}, 50, 20, "y:" },
		in_param{ Point{250, 0}, 50, 20, "" },
		menu{ Point{0, 30}, 120, 20, Menu::vertical, "Figures" },
		next_button{ Point{350, 0}, 70, 20, "Next",
			[](Address, Address pw)
			{
			  reference_to<My_window>(pw).next();
			} },
		quit_button{ Point{x_max() - 70, 0}, 70, 20, "Quit",
					[](Address, Address pw)
					{
					  reference_to<My_window>(pw).quit();
					} }
	{
		attach(in_x);
		attach(in_y);
		attach(in_param);
		menu.attach(new Button{ Point{0, 0}, 0, 0, "Circle",
							   [](Address, Address pw)
							   {
								 reference_to<My_window>(pw).circle();
							   } });
		menu.attach(new Button{ Point{0, 0}, 0, 0, "Square",
							   [](Address, Address pw)
							   {
								 reference_to<My_window>(pw).square();
							   } });
		menu.attach(new Button{ Point{0, 0}, 0, 0, "Regular triangle",
							   [](Address, Address pw)
							   {
								 reference_to<My_window>(pw).triangle();
							   } });
		menu.attach(new Button{ Point{0, 0}, 0, 0, "Regular hexagon",
							   [](Address, Address pw)
							   {
								 reference_to<My_window>(pw).reg_hex();
							   } });
		attach(menu);
		attach(c); //circle
		c.set_color(Color::invisible);
		attach(r); //rectangle
		r.set_color(Color::invisible);
		attach(rtr); //regular triangle
		rtr.set_color(Color::invisible);
		attach(rh); //regular hexagon
		rh.set_color(Color::invisible);
		attach(next_button);
		attach(quit_button);
	}

private:
	In_box in_x, in_y, in_param;
	Menu menu;
	Button next_button, quit_button;
	Circle c{ Point{0, 0}, 20 };
	Graph_lib::Rectangle r{ Point{0, 0}, 40, 40 };
	Regular_polygon rtr{ Point{0, 0}, 3, 20 };	//triangle
	Regular_polygon rh{ Point{0, 0}, 6, 20 };	//hexagon
	bool circle_pressed, square_pressed, triangle_pressed, reg_hex_pressed;
	int x, y, param;

	void circle();
	void square();
	void triangle();
	void reg_hex();
	void draw_circle();
	void draw_square();
	void draw_reg_figure(Regular_polygon& rf);
	void next();
	void quit() { Window::hide(); }
};

void My_window::next()
{
	x = in_x.get_int();
	y = in_y.get_int();
	param = in_param.get_int();

	if (x < 0 || y < 0)
		error("Coordinates should be positive!");
	if (param < 1)
		error("The parameter should be more than zero!");

	if (circle_pressed)
		draw_circle();
	else if (square_pressed)
		draw_square();
	else if (triangle_pressed)
		draw_reg_figure(rtr);
	else if (reg_hex_pressed)
		draw_reg_figure(rh);
	redraw();
}

void My_window::circle()
{
	circle_pressed = true;
	in_param.label = "radius:";
	square_pressed = false;
	triangle_pressed = false;
	reg_hex_pressed = false;
	redraw();
}

void My_window::square()
{
	square_pressed = true;
	in_param.label = "side:";
	circle_pressed = false;
	triangle_pressed = false;
	reg_hex_pressed = false;
	redraw();
}

void My_window::triangle()
{
	triangle_pressed = true;
	in_param.label = "circumradius:";
	square_pressed = false;
	circle_pressed = false;
	reg_hex_pressed = false;
	redraw();
}

void My_window::reg_hex()
{
	reg_hex_pressed = true;
	in_param.label = "circumradius:";
	square_pressed = false;
	circle_pressed = false;
	triangle_pressed = false;
	redraw();
}

void My_window::draw_circle()
{
	Point cur_c = c.center();
	c.move(-cur_c.x + x, -cur_c.y + y);
	c.set_radius(param);
	c.set_color(Color::visible);
}

void My_window::draw_square()
{
	Point cur_c = r.point(0);
	r.move(-cur_c.x + x - param / 2, -cur_c.y + y - param / 2);
	r.change_params(param, param);
	r.set_color(Color::visible);
}

void My_window::draw_reg_figure(Regular_polygon &rf)
//for regular figures from class Regular_polygon
{
	Point cur_c = rf.center();
	rf.move(-cur_c.x + x, -cur_c.y + y);
	rf.change_radius(param);
	rf.set_color(Color::visible);
}

//-----------------------------------------------------------------------------------------------------------------

int main()
try
{
	Point top_left{ 0, 0 }; //will be top left corner of window
	My_window win{ top_left, x_max(), y_max(), "ex5" };
	return gui_main();
}
catch (const std::exception& e)
{
	std::cerr << "Exception: " << e.what() << '\n';
	return 1;
}
catch (...)
{
	cerr << "Some kind of exception\n";
	return 2;
}