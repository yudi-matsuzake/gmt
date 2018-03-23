#include <iostream>

#include <gmt/graphics/render.hpp>
#include <gmt/vec.hpp>
#include <gmt/exception.hpp>

namespace gmt {
class triangle_spinner : public ui_render {
public:
	triangle_spinner()
		: ui_render("Triangle Spinner")
	{}

	virtual ~triangle_spinner()
	{}

	void on_key (int key, int scancode, int action, int)
	{

		try {
			std::cout << "key pressed: " << get_key_name(key, scancode) << std::endl;
		}catch(glfw_unknown_key_name& e){
			std::cout << e.what() << std::endl;
		}

		if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
			set_should_close(true);
		}
	}

	void on_mouse_position(double x, double y)
	{
		std::cout << "(" << x << "," << y << ")" << std::endl;
	}

	void on_cursor_enter()
	{
		std::cout << "Enter" << std::endl;
	}

	void on_cursor_leave()
	{
		std::cout << "Leave" << std::endl;
	}

	void on_mouse_button(int button, int, int)
	{
		std::cout << "mouse button " << button << std::endl;
	}

	void on_scroll(double x, double y)
	{
		std::cout << "scroll " << x << ", " << y << std::endl;
	}

	void renderize()
	{
		float ratio;

		gmt::window_size s = this->get_window_size();

		ratio = s.width / (float) s.height;
		glViewport(0, 0, s.width, s.height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();

	}

};

}

int main()
{

	gmt::triangle_spinner t;

	t.run();

	return 0;
}
