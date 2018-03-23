#include <iostream>

#include <gmt/algorithm/point-in-polygon.hpp>
#include <gmt/graphics/composable_ui.hpp>
#include <gmt/graphics/ui_component/exit_component.hpp>
#include <gmt/graphics/ui_component/single_polygon_component.hpp>
#include <gmt/graphics/ui_component/pan_component.hpp>
#include <gmt/graphics/ui_component/zoom_component.hpp>

class pip : public gmt::composable_ui {
private:
	gmt::polygon2d poly;
	gmt::point2d m;
public:
	pip()
		: gmt::composable_ui("Point in polygon")
	{
		clear_color(black);
		add_component(new gmt::exit_component);
		add_component(new gmt::single_polygon_component(poly));
		add_component(new gmt::zoom_component);
		add_component(new gmt::pan_component);
	}

	~pip()
	{}

	void on_mouse_position(double x, double y)
	{
		gmt::composable_ui::on_mouse_position(x, y);
		m = get_mouse_point(x, y);
	}

	void draw()
	{
		clear();

		if(gmt::point_in_polygon(poly, m))
			color(green);
		else
			color(red);
		plot(poly, GL_LINE_LOOP);
		glPointSize(5);
		color(red);
		plot(m, GL_POINTS);
	}
};

int main()
{
	pip app;
	app.run();
	return 0;
}
