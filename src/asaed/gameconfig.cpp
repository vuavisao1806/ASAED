#include "asaed/gameconfig.hpp"

#include <vector>

#include "util/log.hpp"
#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"

Config::Config() :
	window_size(1280, 960),
	magnification(1.0f),
	debug(false),
	keyboard_config(),
	mouse_config()
{}


void Config::load() {
	ReaderMachine reader("gameconfig.json");
	const ReaderData* data = reader.get_data(0);

	std::vector<int> window;
	if (data->get("window-size", window)) {
		switch (window.size()) {
			case 2:
				window_size.width  = window[0];
				window_size.height = window[1];
				break;

			default:
				throw std::runtime_error("window-size must specify width and height");
				break;
		}
	}

	data->get("magnification", magnification);
	
	int have_debug;
	data->get("debug", have_debug);
	debug = have_debug;
}