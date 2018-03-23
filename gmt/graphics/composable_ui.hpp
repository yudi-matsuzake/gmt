#pragma once

#include <memory>
#include <list>

#include <gmt/graphics/plotter.hpp>
#include <gmt/graphics/rect2d.hpp>
#include <gmt/graphics/ui_component/ui_component.hpp>

namespace gmt {

class composable_ui : public plotter {
protected:
	rect2d viewport;
	rect2d ortho;
	std::list<std::unique_ptr<ui_component>> component_list;

public:
	explicit composable_ui(const std::string& title = "Composable UI",
		int width = 800,
		int height = 600,
		GLFWmonitor* monitor = nullptr,
		GLFWwindow* share = nullptr)
		: plotter(title, width, height, monitor, share)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		window_size winsiz = get_window_size();
		viewport.pos = { 0.0, 0.0 };
		viewport.width = winsiz.width;
		viewport.height = winsiz.height;

		ortho.pos = { 0.0, 0.0 };
		ortho.width = viewport.width;
		ortho.height = viewport.height;
	}

	virtual ~composable_ui()
	{}

	void add_component(ui_component* component)
	{
		component_list.push_back(
			std::unique_ptr<ui_component>(component));
	}

	virtual void reshape()
	{

		gmt::window_size s = get_window_size();

		ortho.width *= s.width/viewport.width;
		ortho.height *= s.height/viewport.height;

		viewport.pos.x() = 0.0;
		viewport.pos.y() =  0.0;
		viewport.width = s.width;
		viewport.height = s.height;

		for(auto& component : component_list)
			component->reshape(*this, s, viewport, ortho);

		glViewport(
			viewport.pos.x(),
			viewport.pos.y(),
			viewport.pos.x() + viewport.width,
			viewport.pos.y() + viewport.height
		);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(ortho.pos.x(),
			ortho.pos.x() + ortho.width,
			ortho.pos.y(), 
			ortho.pos.y() + ortho.height,
			-1.f,
			1.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	}

	virtual void on_key (int key, int scancode, int action, int mods)
	{
		for(auto& component : component_list)
			component->on_key(*this, key, scancode, action, mods);
	}

	virtual void on_mouse_position(double x, double y)
	{
		for(auto& component : component_list)
			component->on_mouse_position(*this, x, y);
	}

	virtual void on_mouse_button(int button, int action, int mods)
	{
		for(auto& component : component_list)
			component->on_mouse_button(*this, button, action, mods);
	}

	virtual void on_cursor_enter()
	{
		for(auto& component : component_list)
			component->on_cursor_enter(*this);
	}

	virtual void on_cursor_leave()
	{
		for(auto& component : component_list)
			component->on_cursor_leave(*this);
	}

	virtual void on_scroll(double xoffset, double yoffset)
	{
		for(auto& component : component_list)
			component->on_scroll(*this, xoffset, yoffset);
	}

	virtual void on_window_size(int width, int height)
	{
		for(auto& component : component_list)
			component->on_window_size(*this, width, height);
	}

	/*
	 * get mouse point based on gmt::point2d
	 * limited by the window size and ortho
	 */
	virtual gmt::point2d get_mouse_point(double x, double y)
	{
		gmt::window_size w = get_window_size();

		y = w.height - y;
		x = (x < 0.0) ? 0.0 : x;
		y = (y < 0.0) ? 0.0 : y;
		x = (x > w.width) ? w.width : x;
		y = (y > w.height) ? w.height : y;

		x *= ortho.width/w.width;
		y *= ortho.height/w.height;

		return ortho.pos + gmt::point2d{ x, y };
	}

	/*
	 * get mouse point based on gmt::point2d
	 * limited by the window size and ortho
	 */
	virtual gmt::point2d get_mouse_point()
	{
		gmt::mouse_position m = get_mouse_position();
		return get_mouse_point(m.x, m.y);
	}

	virtual gmt::rect2d& get_ortho()
	{
		return ortho;
	}

};

}
