#pragma once

#include "ui_component.hpp"

namespace gmt {

class zoom_component : public ui_component {
private:
	double scale, step;
public:
	zoom_component(
		double scale = 1.0,
		double step = 0.1)
		: scale(scale), step(step)
	{}

	~zoom_component()
	{}

	virtual void reshape(	composable_ui&,
				const gmt::window_size& winsiz,
				rect2d&,
				rect2d& ortho)
	{
		ortho.width = winsiz.width*scale;
		ortho.height = winsiz.height*scale;

		double wdiff = (winsiz.width - ortho.width)/2.0;
		double hdiff = (winsiz.height - ortho.height)/2.0;
		ortho.pos.x() = wdiff;
		ortho.pos.y() = hdiff;
	}

	virtual void on_scroll(	composable_ui&,
				double,
				double y)
	{
		double tmp = 1.0 + step*y*(-1.0);
		scale *= tmp;
	}

};

}
