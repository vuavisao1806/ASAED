#include "video/surface_creator.hpp"

#include <SDL_image.h>
#include <sstream>
#include <stdexcept>

#include "util/log.hpp"

SDLSurfacePtr SurfaceCreator::from_file(const std::string& filename) {
	log_debug << "loading image: " << filename << '\n';

	SDLSurfacePtr surface(IMG_Load(filename.c_str()));
	if (!surface) {
		std::ostringstream msg;
		msg << "Couldn't load image " << filename << " :" << SDL_GetError();
		throw std::runtime_error(msg.str());
	}
	else {
		return surface;
	}
}
