
#include <cstdlib>
#include <ctime>
#include <gmt/graphics/composable_ui.hpp>
#include <gmt/graphics/ui_component/exit_component.hpp>
#include <gmt/graphics/ui_component/pan_component.hpp>
#include <gmt/graphics/ui_component/zoom_component.hpp>
#include <gmt/algorithm/convex-hull.hpp>

class convex_hull : public gmt::composable_ui{
protected:
	std::list<gmt::point2d> points;
	gmt::polygon2d ch;

public:
	convex_hull()
		: gmt::composable_ui("Convex Hull")
	{
		clear_color(black);

		add_component(new gmt::exit_component);
		add_component(new gmt::zoom_component);
		add_component(new gmt::pan_component);
	}

	~convex_hull()
	{}

	gmt::point2d get_random_point()
	{
		double rx = ((double)rand())/((double)RAND_MAX);
		double ry = ((double)rand())/((double)RAND_MAX);

		gmt::point2d p;
		p.x() = ortho.pos.x() + ortho.width*rx;
		p.y() = ortho.pos.y() + ortho.height*ry;

		return p;
	}

	void on_mouse_button(int button, int action, int mods)
	{
		composable_ui::on_mouse_button(button, action, mods);

		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
			gmt::point2d m = get_mouse_point();
			points.push_back(m);

			if(points.size() >= 3)
				ch = gmt::merge_hull(points);
		}
	}

	void on_key(int key, int scancode, int action, int mods)
	{
		composable_ui::on_key(key, scancode, action, mods);

		if(key == GLFW_KEY_SPACE &&
			(action == GLFW_REPEAT || action == GLFW_PRESS)){
			auto p = get_random_point();
			points.push_back(p);

			if(points.size() >= 3)
				ch = gmt::merge_hull(points);
		}
	}

	void draw()
	{
		clear();

		color(white);
		plot(points, GL_POINTS);
		if(ch.size()){
			color(blue);
			plot(ch, GL_LINE_LOOP);
			color(green);
			plot(ch, GL_POINTS);
		}
	}
};

int main(int argc, char* argv[])
{
	srand(time(NULL));

	convex_hull app;

	app.run();

	return 0;
}
