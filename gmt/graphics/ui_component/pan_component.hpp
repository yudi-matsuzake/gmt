#pragma once

#include <mutex>

#include <gmt/graphics/composable_ui.hpp>
#include <gmt/graphics/ui_component.hpp>

namespace gmt {

class pan_component : public ui_component {
private:
	typedef enum pan_state {
		WAIT_FIRST,
		PAN
	}pan_state;

	pan_state state;
	gmt::point2d first_point;
	gmt::vec2d pan;
	bool is_pressed;

public:
	pan_component()
		: state(WAIT_FIRST), is_pressed(false)
	{}

	~pan_component()
	{}

	virtual void reshape(	composable_ui&,
				const gmt::window_size&,
				rect2d&,
				rect2d& ortho)
	{
		ortho.pos += pan;
		pan = { 0.0, 0.0 };
	}

	virtual void on_mouse_button(	composable_ui& ui,
					int button,
					int action,
					int)
	{
		if(button == GLFW_MOUSE_BUTTON_MIDDLE){

			if(action == GLFW_PRESS){
				is_pressed = true;

				if(state == WAIT_FIRST){
					state = PAN;
					first_point = ui.get_mouse_point();
				}

			}else if(action == GLFW_RELEASE){
				is_pressed = false;

				state = WAIT_FIRST;
				pan = { ui.get_mouse_point(), first_point };
			}
		}
	}

	virtual void on_mouse_position(	composable_ui& ui,
					double x,
					double y)
	{
		if(is_pressed)
			pan = { ui.get_mouse_point(x, y), first_point };
	}

};

}
