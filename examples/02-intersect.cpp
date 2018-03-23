#include <iostream>

#include <gmt/algorithm.hpp>
#include <gmt/graphics/plotter.hpp>
#include <gmt/segment.hpp>

#define MAX_POINT 4

class intersect : public gmt::plotter {
private:
	size_t point_index;
	int point_ready;
	gmt::point2d points[MAX_POINT];
	gmt::color c;

public:

	intersect()
		: gmt::plotter("intersect"),
		  point_index(0),
		  point_ready(0),
		  c(plotter::white)
	{
		clear_color(plotter::black);
	}

	void on_key(int key, int, int action, int)
	{
		if(action == GLFW_RELEASE){
			switch(key){
			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				set_should_close(true);
				break;
			}
		}
	}

	void on_mouse_button(int button, int action, int)
	{
		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){

			gmt::mouse_position m = get_mouse_position();
			gmt::window_size s = get_window_size();
			points[point_index] = gmt::point2d{ m.x, s.height - m.y };

			point_ready = point_index + 1;
			point_index = (point_index + 1)%MAX_POINT;

			if(point_ready == MAX_POINT){
				gmt::segment2d s0(points[0], points[1]);
				gmt::segment2d s1(points[2], points[3]);

				switch(gmt::intersect(s0, s1)){
				case gmt::intersection::PROPER:
					c = plotter::green;
					std::cout << "PROPER" << std::endl;
					break;
				case gmt::intersection::IMPROPER:
					c = plotter::yellow;
					std::cout << "IMPROPER" << std::endl;
					break;
				case gmt::intersection::NONE:
					c = plotter::red;
					std::cout << "NONE" << std::endl;
					break;
				}

			}else{
				c = plotter::white;
			}

		}
	}

	void handle_events()
	{
		wait_events();
	}

	void draw()
	{

		clear();

		begin(GL_LINES);
		color(c);
		for( int i=0; i < point_ready; i++){
			plot(points[i]);
		}
		end();

	}
};

int main()
{

	intersect app;
	app.run();

	return 0;
}
