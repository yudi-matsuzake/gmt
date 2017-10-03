#pragma once

#include "graphics/composable_ui.hpp"
#include "ui_component.hpp"
#include <iostream>

namespace gmt {

class pan_component : public ui_component {
private:
	typedef enum pan_state {
		WAIT_FIRST,
		PAN
	}pan_state;

	pan_state state;
	gmt::point2d reference_point;
	gmt::point2d pan;
	bool pressed;

public:
	pan_component()
		: state(WAIT_FIRST), pressed(false)
	{}

	~pan_component()
	{}

	virtual void reshape(	composable_ui&,
				const gmt::window_size&,
				rect2d&,
				rect2d& ortho)
	{
		ortho.pos += pan;
	}

	virtual void on_mouse_button(	composable_ui& ui,
					int button,
					int action,
					int)
	{
		if(button == GLFW_MOUSE_BUTTON_MIDDLE){

			if(action == GLFW_PRESS){
				pressed = true;
				if(state == WAIT_FIRST){
					state = PAN;
					reference_point = ui.get_mouse_point();
				}

				pan += reference_point - ui.get_mouse_point();
			}else if(action == GLFW_RELEASE){
				pressed = false;
				state = WAIT_FIRST;
			}
		}
	}

	virtual void on_mouse_position(	composable_ui& ui,
					double,
					double)
	{
		if(pressed)
			pan += reference_point - ui.get_mouse_point();
	}


};

}
