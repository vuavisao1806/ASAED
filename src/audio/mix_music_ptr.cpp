#include "audio/mix_music_ptr.hpp"

#include <sstream>
#include <stdexcept>

#include "util/log.hpp"

MixMusicPtr MixMusicCreator::from_file(const std::string& filename) {
	log_debug << "loading mix_music: " << filename << '\n';

	MixMusicPtr mix_music(Mix_LoadMUS(filename.c_str()));
	if (!mix_music) {
		std::ostringstream msg;
		msg << "Couldn't load mix_music " << filename << " :" << Mix_GetError();
		throw std::runtime_error(msg.str());
	}
	else {
		return mix_music;
	}
}