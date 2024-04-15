#ifndef HEADER_ASAED_AUDIO_MIX_CHUNK_PTR_HPP
#define HEADER_ASAED_AUDIO_MIX_CHUNK_PTR_HPP

#include <SDL_mixer.h>
#include <memory>
#include <string>

/** This class wraps Mix_Chunk* to do things */

class MixChunkPtr final {
private:
	Mix_Chunk* m_mix_chunk;

public:
	MixChunkPtr() :
		m_mix_chunk(nullptr)
	{}

	~MixChunkPtr()
	{
		Mix_FreeChunk(m_mix_chunk);
	}

	explicit MixChunkPtr(Mix_Chunk* mix_chunk) :
		m_mix_chunk(mix_chunk)
	{}

	MixChunkPtr(MixChunkPtr&& other) noexcept :
		m_mix_chunk(other.m_mix_chunk)
	{}

	MixChunkPtr& operator=(MixChunkPtr&& other) noexcept {
		if(this != &other) {
			m_mix_chunk = other.m_mix_chunk;
			other.m_mix_chunk = nullptr;
		}
		return (*this);
	}

private:
	MixChunkPtr(const MixChunkPtr&) = delete;
	MixChunkPtr& operator=(const MixChunkPtr&) = delete;

public:

	Mix_Chunk& operator*() {
		return *m_mix_chunk; 
	}
	
	const Mix_Chunk& operator*() const {
		return *m_mix_chunk; 
	}

	Mix_Chunk* operator->() {
		return m_mix_chunk;
	}

	const Mix_Chunk* operator->() const {
		return m_mix_chunk;
	}
	
	void reset(Mix_Chunk* mix_chunk) {
		Mix_FreeChunk(m_mix_chunk);
		m_mix_chunk = mix_chunk;
	}

	Mix_Chunk* get() {
		return m_mix_chunk;
	}

	operator bool() {
		return m_mix_chunk;
	}
};

/**
 * This class to create MixChunkPtr from image load
 * It isn't manager because it doesn't contain responsibility to save
*/

class MixChunkCreator final {
public:
	static MixChunkPtr from_file(const std::string& filename);
};



#endif