#pragma once

#include "ui_component.hpp"

namespace gmt {

class zoom_component : public ui_component {
private:
	double scale, step;
	bool update;
public:
	zoom_component(
		double step = 0.1)
		: scale(1.0), step(step), update(false)
	{}

	~zoom_component()
	{}

	virtual void reshape(	composable_ui&,
				const gmt::window_size&,
				rect2d&,
				rect2d& ortho)
	{
		if(update){
			update = false;

			double old_ortho_w = ortho.width;
			double old_ortho_h = ortho.height;
			ortho.width = ortho.width*scale;
			ortho.height = ortho.height*scale;

			double wdiff = (old_ortho_w - ortho.width)/2.0;
			double hdiff = (old_ortho_h - ortho.height)/2.0;
			ortho.pos.x() += wdiff;
			ortho.pos.y() += hdiff;
		}
	}

	virtual void on_scroll(	composable_ui&,
				double,
				double y)
	{
		scale = 1.0 + step*y*(-1.0);
		/* scale *= tmp; */
		update = true;
	}

};

}
