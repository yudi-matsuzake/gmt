#pragma once
#include <gmt/graphics/plotter.hpp>
#include <gmt/graphics/rect2d.hpp>

namespace gmt {

class composable_ui;

class ui_component {
public:
	ui_component()
	{}

	virtual ~ui_component()
	{}

	virtual void reshape(	composable_ui&,
				const gmt::window_size&,
				rect2d&,
				rect2d&)
	{}

	virtual void on_key (	composable_ui&,
				int,
				int,
				int,
				int)
	{}

	virtual void on_mouse_position(	composable_ui&,
					double,
					double)
	{}

	virtual void on_mouse_button(	composable_ui&,
					int,
					int,
					int)
	{}

	virtual void on_cursor_enter(composable_ui&)
	{}

	virtual void on_cursor_leave(composable_ui&)
	{}

	virtual void on_scroll(	composable_ui&,
				double,
				double)
	{}

	virtual void on_window_size(
				composable_ui&,
				int,
				int)
	{}

};

}
