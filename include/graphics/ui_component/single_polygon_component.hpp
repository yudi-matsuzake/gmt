#pragma once

#include "polygon.hpp"
#include "graphics/composable_ui.hpp"
#include "ui_component.hpp"

namespace gmt {

class single_polygon_component : public ui_component {
private:
	polygon2d& poly;
	int button;
	int action;
public:
	single_polygon_component(gmt::polygon2d& poly,
			int button = GLFW_MOUSE_BUTTON_LEFT,
			int action = GLFW_RELEASE)
		: poly(poly),
		  button(button),
		  action(action)
	{}

	~single_polygon_component()
	{}

	virtual void on_mouse_button(	composable_ui& ui,
					int button,
					int action,
					int)
	{
		if(this->button == button && this->action == action)
			poly.push_back(ui.get_mouse_point());
	}
};

}
