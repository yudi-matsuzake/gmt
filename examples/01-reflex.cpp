#include <iostream>

#include <gmt/graphics/plotter.hpp>
#include <gmt/vec.hpp>
#include <gmt/polygon.hpp>
#include <gmt/algorithm.hpp>

class reflex : public gmt::plotter {
	gmt::polygon2d poly;
public:

	reflex ()
	 : gmt::plotter("reflex")
	{
		clear_color(plotter::black);
	}

	~reflex()
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
			if(gmt::is_reflex(poly, i))
				color(plotter::red);
			else
				color(plotter::white);

			plot(poly[i]);
		}
		end();
	}

};

int main()
{
	reflex app;

	app.run();

	return 0;
}
