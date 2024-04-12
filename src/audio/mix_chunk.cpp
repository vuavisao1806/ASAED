#include "audio/mix_chunk.hpp"

#include <sstream>
#include <stdexcept>

#include "util/log.hpp"

MixChunk::MixChunk(Mix_Chunk* mix_chunk) :
	m_mix_chunk(std::move(mix_chunk)),
	m_volume(MIX_MAX_VOLUME)
{}

MixChunk::~MixChunk() {
	Mix_FreeChunk(m_mix_chunk);
}

void MixChunk::play(int volume, int loops) {
	// trolling
	Mix_VolumeChunk(m_mix_chunk, std::min(volume, m_volume));
	Mix_PlayChannel(-1, m_mix_chunk, loops);
}

void MixChunk::set_volume(int volume) { m_volume = volume; }
const int& MixChunk::get_volume() { return m_volume; }

std::unique_ptr<MixChunk> MixChunkCreator::from_file(const std::string& filename) {
	log_debug << "loading mix_chunk: " << filename << '\n';

	Mix_Chunk* mix_chunk = Mix_LoadWAV(filename.c_str());
	if (!mix_chunk) {
		std::ostringstream msg;
		msg << "Couldn't load mix_chunk " << filename << " :" << Mix_GetError();
		throw std::runtime_error(msg.str());
	}
	else {
		auto new_mix_chunk = std::make_unique<MixChunk>(mix_chunk);
		return new_mix_chunk;
	}
}