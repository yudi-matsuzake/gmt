#pragma once

#include "ui_component.hpp"

namespace gmt {

class zoom_component : public ui_component {
protected:
	double scale, step;
	bool update;
public:
	zoom_component(double step = 0.1)
		: scale(1.0), step(step), update(false)
	{}

	~zoom_component()
	{}

	virtual void reshape(	composable_ui& ui,
				const gmt::window_size&,
				rect2d&,
				rect2d& ortho)
	{
		if(update){
			update = false;

			point2d m0 = ui.get_mouse_point();

			ortho.width = ortho.width*scale;
			ortho.height = ortho.height*scale;

			point2d m1 = ui.get_mouse_point();
			vec2d translation = { m0, m1 };

			ortho.pos -= translation;
		}
	}

	virtual void on_scroll(	composable_ui&,
				double,
				double y)
	{
		scale = 1.0 + step*y*(-1.0);
		if(y != 0.0)
			update = true;
	}

};

}
