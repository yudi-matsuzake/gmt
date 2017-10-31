#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "graphics/composable_ui.hpp"
#include "ui_component.hpp"

namespace gmt {

class img_component : public ui_component {
protected:
	std::vector<vertex2d>& vertexes;
public:

	static double char_to_double(uchar c)
	{
		return ((double)c)/255.0;
	}

	static gmt::color pixel_to_color(const uchar* p, size_t n_channels)
	{
		if(p == nullptr)
			return gmt::color();

		switch(n_channels){
		case 4:
			return color(
				char_to_double(p[2]),
				char_to_double(p[1]),
				char_to_double(p[0]),
				char_to_double(p[3])
			);
			break;
		case 3:
			return color(
				char_to_double(p[2]),
				char_to_double(p[1]),
				char_to_double(p[0])
			);
			break;
		}

		return color();
	}

	img_component(const std::string& s, std::vector<vertex2d>& vertexes)
		: img_component(cv::imread(s.c_str(), CV_LOAD_IMAGE_UNCHANGED),
		  vertexes)
	{}

	img_component(const cv::Mat& img, std::vector<vertex2d>& vertexes)
		: vertexes(vertexes)
	{
		cv::Mat tmp;
		if(img.channels() == 1)
			cv::cvtColor(img, tmp, cv::COLOR_GRAY2RGBA);
		else
			img.copyTo(tmp);

		for(int i=0; i<tmp.rows; i++) {
		
			const uchar* ptr = tmp.ptr<uchar>(i);
			for(int j=0; j<tmp.cols; j++){

				vertex2d v = { 
					(double) j,
					(double) tmp.rows - i
				};

				v.color = pixel_to_color(ptr, tmp.channels());
				vertexes.push_back(v);

				ptr+=tmp.channels();
			}
		}
	}

	~img_component()
	{}
};

}
