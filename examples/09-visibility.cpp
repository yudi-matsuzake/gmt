#include <iostream>
#include <vector>

#include <gmt/graphics/plotter.hpp>
#include <gmt/algorithm/polygon-visibility.hpp>

class visibility : public gmt::plotter {
	std::vector<gmt::polygon2d> poly_list;
	size_t index;
	bool poly_finish;

public:
	visibility() : gmt::plotter("visibility"),
		index(0),
		poly_finish(false)
	{
		clear_color(plotter::black);

		gmt::window_size winsiz = get_window_size();
		on_window_size(winsiz.width, winsiz.height);
		poly_list.push_back(gmt::polygon2d());
	}

	~visibility()
	{}

	void on_window_size(int width, int height)
	{
		gmt::polygon2d winpoly = {
			gmt::point2d{ -1, -1 },
			gmt::point2d{ -1, static_cast<double>(height)+1 },
			gmt::point2d{
				static_cast<double>(width)+1,
				static_cast<double>(height)+1
			},
			gmt::point2d{
				static_cast<double>(width)+1,
				-1
			}
		};

		if(poly_list.size() == 0)
			poly_list.push_back(winpoly);
		else
			poly_list[0] = winpoly;

		if(index == 0)
			index = 1;
	}

	void next_poly()
	{
		if(!poly_finish){
			poly_list.push_back(gmt::polygon2d());
			index++;
		}
	}

	void clear_poly_list()
	{
		poly_list.clear();
		index = 0;
		poly_finish = false;
	}

	gmt::point2d get_mouse_point()
	{
		gmt::mouse_position mouse_pos = get_mouse_position();
		gmt::window_size   window_siz = get_window_size();

		return gmt::point2d{
			mouse_pos.x,
			window_siz.height - mouse_pos.y
		};
	}

	void finish_poly()
	{
		poly_finish = true;
	}

	void on_mouse_button(int button, int action, int)
	{
		if(!poly_finish
			&& button == GLFW_MOUSE_BUTTON_LEFT
			&& action == GLFW_RELEASE){

			poly_list[index].push_back(get_mouse_point());
		}
	}

	void on_key(int key, int, int action, int)
	{
		if(action == GLFW_RELEASE){
			switch(key){
			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				set_should_close(true);
				break;
			case GLFW_KEY_N:
				next_poly();
				break;
			case GLFW_KEY_C:
				clear_poly_list();
				break;
			case GLFW_KEY_ENTER:
				finish_poly();
				break;
			}
		}
	}

	void draw()
	{
		clear();
		color(plotter::white);
		plot(poly_list, GL_LINE_LOOP);

		if(poly_finish){
			gmt::point2d mpos = get_mouse_point();
			gmt::window_size winsiz = get_window_size();

			mpos.x() = (mpos.x() < 0)? 0.0 : mpos.x();
			mpos.x() = (mpos.x() > winsiz.width)? winsiz.width : mpos.x();
			mpos.y() = (mpos.y() < 0)? 0.0 : mpos.y();
			mpos.y() = (mpos.y() > winsiz.height)? winsiz.height : mpos.y();

			gmt::polygon2d visibility = gmt::polygon_visibility(
					mpos,
					poly_list);

			for(size_t i=1; i<=visibility.size(); i++){
				/*
				 * polygon
				 */
				color(gmt::color(0.25, 0.25, 0.0, 1.0));
				begin(GL_TRIANGLES);
				plot(mpos);
				plot(visibility[i-1]);
				plot(visibility[i%visibility.size()]);
				end();

				/*
				 * dashed lines
				 */
				color(plotter::gray);
				glPushAttrib(GL_ENABLE_BIT); 
				glLineStipple(4, 0xAAAA); 
				glEnable(GL_LINE_STIPPLE);
				begin(GL_LINES);
				plot(mpos);
				plot(visibility[i-1]);
				plot(mpos);
				plot(visibility[i%visibility.size()]);
				end();
				glPopAttrib();
			}

			color(plotter::yellow);
			plot(visibility, GL_LINE_LOOP);

			color(plotter::yellow);
			glPointSize(5);
			plot(visibility, GL_POINTS);

		}
	}
};

int main()
{

	visibility app;
	app.run();

	return 0;
}
