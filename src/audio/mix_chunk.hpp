#ifndef HEADER_ASAED_AUDIO_MIX_CHUNK_HPP
#define HEADER_ASAED_AUDIO_MIX_CHUNK_HPP

#include <SDL_mixer.h>
#include <memory>
#include <string>

/** This class wraps Mix_Chunk* to do things */

class MixChunk final {
private:
	Mix_Chunk* m_mix_chunk;
	int m_volume;

public:
	MixChunk(Mix_Chunk* mix_chunk);
	~MixChunk();

private:
	MixChunk(const MixChunk&) = delete;
	MixChunk& operator=(const MixChunk&) = delete;

public:
	void play(int volume, int loops = 0);

	const int& get_volume();
	void set_volume(int volume);
};


/**
 * This class to create MixChunk from image load
 * It isn't manager because it doesn't contain responsibility to save
*/

class MixChunkCreator final {
public:
	static std::unique_ptr<MixChunk> from_file(const std::string& filename);
};


#endif