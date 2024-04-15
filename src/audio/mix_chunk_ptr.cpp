#include "audio/mix_chunk_ptr.hpp"

#include <sstream>
#include <stdexcept>

#include "util/log.hpp"

MixChunkPtr MixChunkCreator::from_file(const std::string& filename) {
	log_debug << "loading mix_chunk: " << filename << '\n';

	MixChunkPtr mix_chunk(Mix_LoadWAV(filename.c_str()));
	if (!mix_chunk) {
		std::ostringstream msg;
		msg << "Couldn't load mix_chunk " << filename << " :" << Mix_GetError();
		throw std::runtime_error(msg.str());
	}
	else {
		return mix_chunk;
	}
}