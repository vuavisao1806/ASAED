#ifndef HEADER_ASAED_AUDIO_SOUND_MANAGER_HPP
#define HEADER_ASAED_AUDIO_SOUND_MANAGER_HPP

#include <SDL_mixer.h>

#include <map>
#include <memory>
#include <string>

#include "util/currenton.hpp"

class MixChunk;
class MixMusicPtr;
class ReaderData;

enum class TypeSound {
	MUSIC,
	CHUNK
};

class SoundManager final : public Currenton<SoundManager> {
private:
	std::string filename;
	std::map<std::string, MixMusicPtr> m_mix_musics;
	std::map<std::string, std::unique_ptr<MixChunk>> m_mix_chunks;

	std::string current_music_name;
	bool m_pause_music;
	int m_volume;

public:
	SoundManager();
	~SoundManager() override;

private:
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

public:
	void play_music(const std::string& name);
	void pause_music();

	void play_chunk(const std::string& name, int loops = 0);

	const int& get_volume();
	void set_volume(int volume);

	Mix_Music* get_music(const std::string& name);
	const std::unique_ptr<MixChunk>& get_chunk(const std::string& name);
	
private:
	void parse(const ReaderData* data);
};

TypeSound from_string_to_type_sound(const std::string& name);

#endif