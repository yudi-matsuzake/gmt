#pragma once

#include <GLFW/glfw3.h>

#include <memory>
#include <map>

#include "exception.hpp"

namespace gmt {

struct window_size {
	int width;
	int height;
};

struct mouse_position {
	double x;
	double y;

	friend std::ostream& operator<<(
		std::ostream& o,
		const mouse_position& mpos)
	{
		o	<< "mouse_position (x: " << mpos.x 
			<< ", y: " << mpos.y << ")";
		return o;
	}

};

class render {
public:

	render( const std::string& title = "Render",
		int width = 800,
		int height = 600,
		GLFWmonitor* monitor = nullptr,
		GLFWwindow* share = nullptr)
		: window(init(
			title.c_str(),
			width,
			height,
			monitor,
			share),
		  destroy_window_and_terminate)
	{
		// TODO: make glfw_counter thread safe!
		glfw_counter++;
		make_current_context();
	}

	virtual ~render()
	{}

	void make_current_context()
	{
		glfwMakeContextCurrent(window.get());
	}

	bool should_close()
	{
		return glfwWindowShouldClose(window.get());
	}

	void set_should_close(bool should)
	{
		glfwSetWindowShouldClose(window.get(), should);
	}

	void swap_buffers()
	{
		glfwSwapBuffers(window.get());
	}

	void poll_events()
	{
		glfwPollEvents();
	}

	void set_point_size(double size)
	{
		glPointSize(size);
	}

	void wait_events(double timeout = -1.0)
	{
		if(timeout == -1.0)
			glfwWaitEvents();
		else
			glfwWaitEventsTimeout(timeout);
	}

	double get_time() const
	{
		return glfwGetTime();
	}

	window_size get_window_size() const
	{
		int w, h;
		glfwGetFramebufferSize(window.get(), &w, &h);
		window_size s = { .width = w, .height = h };

		return s;
	}

	/*
	 * function that will be called in the main loop, and
	 * will manage the events, either by polling events or
	 * waiting (must be override)
	 */
	virtual void handle_events()
	{
		poll_events();
	}

	virtual void renderize() = 0;

	void run()
	{
		while(!should_close()){
			make_current_context();
			renderize();
			swap_buffers();
			handle_events();
		}
	}

protected:
	static std::size_t glfw_counter;

	std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window;

	static void error_callback(int error, const char* description)
	{
		throw glfw_error(error, description);
	}

	static GLFWwindow* init(
		const std::string& title,
		int width,
		int height,
		GLFWmonitor* monitor,
		GLFWwindow* share)
	{
		if(!glfwInit())
			throw exception("Fail to initialize GLFW");

		glfwSetErrorCallback(error_callback);

		GLFWwindow* w = glfwCreateWindow(
			width,
			height,
			title.c_str(),
			monitor,
			share);

		if(w == nullptr){
			terminate();
			throw exception("Fail to create the window");
		}

		return w;
	}

	static void terminate()
	{
		if(--glfw_counter == 0)
			glfwTerminate();
	}

	static void destroy_window_and_terminate(GLFWwindow* w)
	{
		glfwDestroyWindow(w);
		terminate();
	}

};

class ui_render : public render {
private:
	static std::map<GLFWwindow*, ui_render*> window_to_render;

	static ui_render* get_render_by_window(GLFWwindow* window)
	{
		auto it = window_to_render.find(window);

		if(it == window_to_render.end())
			throw exception("Could not find a render to callback");

		return it->second;
	}

	static void key_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		ui_render* render = get_render_by_window(window);
		render->on_key(key, scancode, action, mods);
	}

	static void cursor_position_callback(GLFWwindow* window, double x, double y)
	{
		ui_render* render = get_render_by_window(window);
		render->on_mouse_position(x, y);
	}

	static void cursor_enter_callback(GLFWwindow* window, int entered)
	{
		ui_render* render = get_render_by_window(window);

		if(entered)
			render->on_cursor_enter();
		else
			render->on_cursor_leave();
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		ui_render* render = get_render_by_window(window);
		render->on_mouse_button(button, action, mods);
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		ui_render* render = get_render_by_window(window);
		render->on_scroll(xoffset, yoffset);
	}

	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		ui_render* render = get_render_by_window(window);
		render->on_window_size(width, height);
	}

public:
	ui_render( const std::string& title = "Render",
		int width = 800,
		int height = 600,
		GLFWmonitor* monitor = nullptr,
		GLFWwindow* share = nullptr)
		: render(title, width, height, monitor, share)
	{
		std::pair<GLFWwindow*, ui_render*> p(this->window.get(), this);
		window_to_render.insert(p);

		glfwSetInputMode(window.get(), GLFW_STICKY_MOUSE_BUTTONS, 1);
		glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, 1);

		glfwSetKeyCallback(this->window.get(), key_input_callback);
		glfwSetCursorPosCallback(this->window.get(), cursor_position_callback);
		glfwSetCursorEnterCallback(this->window.get(), cursor_enter_callback);
		glfwSetMouseButtonCallback(this->window.get(), mouse_button_callback);
		glfwSetScrollCallback(this->window.get(), scroll_callback);
		glfwSetWindowSizeCallback(this->window.get(), window_size_callback);

	}

	virtual ~ui_render()
	{}

	mouse_position get_mouse_position() const
	{
		mouse_position pos;
		glfwGetCursorPos(window.get(), &(pos.x), &(pos.y));

		return pos;
	}
	
	int get_key_state(int key)
	{
		return glfwGetKey(window.get(), key);
	}

	int get_mouse_button_state(int button)
	{
		return glfwGetMouseButton(window.get(), button);
	}

	const std::string get_key_name(int key, int scancode = 0)
	{
		const char* c = glfwGetKeyName(key, scancode);

		if(c == nullptr)
			throw glfw_unknown_key_name();

		return c;
	}

	virtual void on_key (int, int, int, int)
	{}

	virtual void on_mouse_position(double, double)
	{}

	virtual void on_mouse_button(int, int, int)
	{}

	virtual void on_cursor_enter()
	{}

	virtual void on_cursor_leave()
	{}

	virtual void on_scroll(double, double)
	{}

	virtual void on_window_size(int, int)
	{}

};

// static globals
std::size_t render::glfw_counter = 0;
std::map<GLFWwindow*, ui_render*> ui_render::window_to_render;

}
