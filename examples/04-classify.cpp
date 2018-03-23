#include <iostream>

#include <gmt/graphics/plotter.hpp>
#include <gmt/vec.hpp>
#include <gmt/polygon.hpp>
#include <gmt/algorithm.hpp>

class classify : public gmt::plotter {
	gmt::polygon2d poly;
public:

	classify ()
	 : gmt::plotter("classify")
	{
		clear_color(plotter::black);
	}

	~classify()
	{}

	void on_key(int key, int, int action, int)
	{
		if(action == GLFW_RELEASE){
			switch(key){
			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				set_should_close(true);
				break;
			case GLFW_KEY_C:
				poly.clear();
				break;
			}
		}
	}

	void on_mouse_button(int button, int action, int)
	{
		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
			gmt::mouse_position mouse_pos = get_mouse_position();
			gmt::window_size   window_siz = get_window_size();

			poly.push_back(gmt::vec2d{
					mouse_pos.x,
					window_siz.height - mouse_pos.y
				});
		}
	}

	void handle_events()
	{
		wait_events();
	}

	void draw()
	{
		clear();

		begin(GL_LINE_LOOP);
		for(std::size_t i=0; i<poly.size(); i++){
			switch(gmt::classify(poly, i)){
				case gmt::REGULAR:
					color(white);
					break;
				case gmt::START:
					color(green);
					break;
				case gmt::END:
					color(red);
					break;
				case gmt::SPLIT:
					color(blue);
					break;
				case gmt::MERGE:
					color(yellow);
					break;
			}

			plot(poly[i]);
		}
		end();
	}

};

int main()
{
	classify app;

	app.run();

	return 0;
}
