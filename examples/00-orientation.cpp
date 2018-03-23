#include <iostream>

#include <gmt/algorithm.hpp>
#include <gmt/graphics/plotter.hpp>

#define MAX_POINT 3

class orientation : public gmt::plotter {
private:
	size_t point_index;
	int point_ready;
	gmt::point2d points[MAX_POINT];
	gmt::color c;

public:

	orientation()
		: gmt::plotter("orientation"),
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

			std::cout << "v" << point_index
				  << " (" << m.x << ", " << m.y << ")" << std::endl;

			points[point_index] = gmt::point2d{ m.x, s.height - m.y };
			point_ready = point_index + 1;
			point_index = (point_index + 1)%MAX_POINT;

			if(point_ready == 3){
				gmt::direction d = gmt::direction_in(
							points[0],
							points[1],
							points[2]);

				switch(d){
				case gmt::LEFT:
					c = plotter::red;
					std::cout << "LEFT" << std::endl;
					break;
				case gmt::RIGHT:
					c = plotter::blue;
					std::cout << "RIGHT" << std::endl;
					break;
				case gmt::ON:
					c = plotter::yellow;
					std::cout << "ON" << std::endl;
					break;
				};

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
		for( int i=0; i < (point_ready-1); i++){
			plot(points[i]);
			plot(points[i+1]);
		}
		end();

	}
};

int main()
{

	orientation ex;
	ex.run();

	return 0;
}
