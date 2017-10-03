#pragma once

#include <vector>

#include "graphics/composable_ui.hpp"
#include "ui_component.hpp"

namespace gmt {

class exit_component : public ui_component {
private:
	std::vector<int> key;
	int action;
public:
	exit_component(
		const std::vector<int>& key = {
			GLFW_KEY_ESCAPE,
			GLFW_KEY_Q
		},
		int action = GLFW_RELEASE)
		: key(key), action(action)
	{}

	virtual ~exit_component()
	{}

	virtual void on_key (	composable_ui& ui,
				int key,
				int,
				int action,
				int)
	{
		if(this->action == action)
			for(auto& k : this->key)
				if(k ==  key)
					ui.set_should_close(true);
	}
};

}
