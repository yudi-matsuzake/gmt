#include <iostream>

#include <gmt/algorithm/point-in-polygon.hpp>
#include <gmt/graphics/composable_ui.hpp>
#include <gmt/graphics/ui_component/exit_component.hpp>
#include <gmt/graphics/ui_component/single_polygon_component.hpp>
#include <gmt/graphics/ui_component/pan_component.hpp>
#include <gmt/graphics/ui_component/zoom_component.hpp>

class pip : public gmt::composable_ui {
private:
	gmt::polygon_with_holes2d poly;

	gmt::point2d m;
public:
	pip()
		: gmt::composable_ui("Point in polygon with holes")
	{
		clear_color(black);
		add_component(new gmt::exit_component);
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

	void on_key(int key, int scancode, int action, int mods)
	{
		gmt::composable_ui::on_key(key, scancode, action, mods);

		if(key == GLFW_KEY_N && action == GLFW_RELEASE)
			poly.add_hole(gmt::polygon2d());
	}

	void on_mouse_button(int button, int action, int mods)
	{
		gmt::composable_ui::on_mouse_button(button, action, mods);

		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
			if(poly.holes().size())
				poly.hole(poly.holes().size() - 1).push_back(m);
			else
				poly.push_back(m);
		}
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
