#ifndef HEADER_ASAED_AUDIO_MIX_MUSIC_PTR_HPP
#define HEADER_ASAED_AUDIO_MIX_MUSIC_PTR_HPP

#include <SDL_mixer.h>
#include <string>

/** This class wraps Mix_Music* to do things */

class MixMusicPtr final {
private:
	Mix_Music* m_mix_music;

public:
	MixMusicPtr() :
		m_mix_music(nullptr)
	{}

	~MixMusicPtr()
	{
		Mix_FreeMusic(m_mix_music);
	}

	explicit MixMusicPtr(Mix_Music* mix_music) :
		m_mix_music(mix_music)
	{}

	MixMusicPtr(MixMusicPtr&& other) noexcept :
		m_mix_music(other.m_mix_music)
	{}

	MixMusicPtr& operator=(MixMusicPtr&& other) noexcept {
		if(this != &other) {
			m_mix_music = other.m_mix_music;
			other.m_mix_music = nullptr;
		}
		return (*this);
	}

private:
	MixMusicPtr(const MixMusicPtr&) = delete;
	MixMusicPtr& operator=(const MixMusicPtr&) = delete;

public:

	Mix_Music& operator*() {
		return *m_mix_music; 
	}
	
	const Mix_Music& operator*() const {
		return *m_mix_music; 
	}

	Mix_Music* operator->() {
		return m_mix_music;
	}

	const Mix_Music* operator->() const {
		return m_mix_music;
	}
	
	void reset(Mix_Music* mix_music) {
		Mix_FreeMusic(m_mix_music);
		m_mix_music = mix_music;
	}

	Mix_Music* get() {
		return m_mix_music;
	}

	operator bool() {
		return m_mix_music;
	}
};

/**
 * This class to create MixMusicPtr from image load
 * It isn't manager because it doesn't contain responsibility to save
*/

class MixMusicCreator final {
public:
	static MixMusicPtr from_file(const std::string& filename);
};



#endif