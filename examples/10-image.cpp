#include <iostream>

#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <gmt/algorithm/distance.hpp>
#include <gmt/graphics/composable_ui.hpp>
#include <gmt/graphics/ui_component/exit_component.hpp>
#include <gmt/graphics/ui_component/pan_component.hpp>
#include <gmt/graphics/ui_component/zoom_component.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define USAGE " <img>"

class img_mixer : public gmt::composable_ui {
private:

	class vertex {
	public:
		vertex()
		{}

		vertex(const gmt::point2d& p,
			const gmt::color& color = gmt::color())
			: static_position(p),
			  current_position(p),
			  color(color),
			  velocity{ 0.0, 0.0 }
		{}

		~vertex()
		{}

		bool is_moving() const
		{
			return !(static_position == current_position);
		}

		gmt::point2d static_position;
		gmt::point2d current_position;
		gmt::color color;

		gmt::vec2d acceleration;
		gmt::vec2d velocity;
	};

	const double alpha_threshold = 0.1;
	double mouse_radius = 100.0;
	gmt::point2d mouse_point;
	bool mouse_radius_enabled;

	std::vector<vertex> vertexes;

	cv::Mat& img;
public:

	img_mixer(cv::Mat& img)
		: composable_ui("Image Mixer"),
		  mouse_radius_enabled(true),
		  img(img)
	{
		clear_color((red*2.0 + white)/3.0);
		add_component(new gmt::exit_component);
		add_component(new gmt::zoom_component);
		add_component(new gmt::pan_component);

		for(int i=0; i<img.rows; i++) {
		
			uchar* ptr = img.ptr<uchar>(i);
			for(int j=0; j<img.cols; j++ ) {

				if(img.channels() < 4 || d(ptr[3]) >= alpha_threshold){
					vertex v(
						gmt::point2d{
							(double) j,
							(double) img.rows - i
						},
						gmt::color(d(ptr[2]), d(ptr[1]), d(ptr[0]))
					);

					vertexes.push_back(v);
				}

				ptr+=img.channels();
			}
		}
		glPointSize(2.5);
	}

	~img_mixer()
	{}

	double d(uchar c)
	{
		return ((double)c)/255.0;
	}

	double drand()
	{
		return ((double)rand())/RAND_MAX;
	}

	void on_key(int key, int scancode, int action, int mods)
	{
		composable_ui::on_key(key, scancode, action, mods);

		if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){
			for(auto& v : vertexes){
				v.current_position.x() = ortho.pos.x()
							+ ortho.width*drand();
				v.current_position.y() = ortho.pos.y()
							+ ortho.height*drand();
			}
		}else if(key == GLFW_KEY_SPACE && action == GLFW_REPEAT){
			gmt::point2d m = get_mouse_point();

			for(int i=0; i<vertexes.size()*0.2; i++)
				vertexes[rand()%vertexes.size()].current_position = m;
		}else if(key == GLFW_KEY_R && action == GLFW_RELEASE){
			mouse_radius_enabled = !mouse_radius_enabled;
		}else if(key == GLFW_KEY_PAGE_UP &&
				(action == GLFW_PRESS || action == GLFW_REPEAT)){
			mouse_radius *= 1.1;
		}else if(key == GLFW_KEY_PAGE_DOWN &&
				(action == GLFW_PRESS || action == GLFW_REPEAT)){
			mouse_radius *= 0.9;
		}
	}

	void on_mouse_button(int button, int action, int mods)
	{
		composable_ui::on_mouse_button(button, action, mods);

		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
			gmt::point2d m = get_mouse_point();
			for(auto& v : vertexes){
				v.current_position = m;
			}
		}
	}

	void on_mouse_position(double x, double y)
	{
		composable_ui::on_mouse_position(x, y);
		mouse_point = get_mouse_point(x, y);
	}

	void update_vertexes()
	{
		for(auto& v : vertexes){

			gmt::vec2d tmp(
				v.static_position
				- v.current_position
				- v.velocity);

			double f = 5.0;
			if(tmp.norm() < f)
				v.acceleration = tmp;
			else
				v.acceleration = tmp.versor()*f;

			v.velocity += v.acceleration;
			v.current_position += v.velocity;

			double distance = gmt::distance(
				mouse_point,
				v.current_position
			);

			if(mouse_radius_enabled && distance <= mouse_radius){
				gmt::vec2d tmp = {
					mouse_point,
					v.current_position
				};

				double colision_size = mouse_radius - distance;
				gmt::vec2d colision = tmp.versor()*colision_size;

				/* v.current_position += tmp + colision; */
				v.velocity += colision;

			}

		}
	}

	void sort_to_draw()
	{
		std::list<vertex> l;
		size_t moving_index = 0;

		for(size_t i=0; i<vertexes.size() ; i++){
			if(vertexes[i].is_moving()){
				l.push_back(vertexes[i]);
			}else{
				std::swap(vertexes[i], vertexes[moving_index++]);
			}
		}

		for(auto& v : l){
			vertexes[moving_index++] = v;
		}
	}

	void draw()
	{
		clear();

		update_vertexes();

		sort_to_draw();

		begin(GL_POINTS);
		for(auto& v : vertexes){
			color(v.color);
			plot(v.current_position);
		}
		end();

	}
};

int main(int argc, char* argv[])
{
	srand(time(nullptr));

	if(argc != 2){
		std::cerr << argv[0] << USAGE << std::endl;
		return 1;
	}

	cv::Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);

	if(img.data == nullptr){
		std::cerr << "Could not open the image" << std::endl;
		return 1;
	}

	img_mixer app(img);
	app.run();

	return 0;
}
