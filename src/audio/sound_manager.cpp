#include "audio/sound_manager.hpp"

#include <sstream>

#include "audio/mix_chunk_ptr.hpp"
#include "audio/mix_music_ptr.hpp"
#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"

SoundManager::SoundManager() :
	filename("data/music/music-list.json"),
	m_mix_musics(),
	m_mix_chunks(),
	current_music_name(),
	m_pause_music(false),
	m_volume_music(100),
	m_volume_sound(100)
{
	ReaderMachine reader(filename);
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse(reader.get_data(i));
	}
}

SoundManager::~SoundManager()
{}


void SoundManager::play_music(const std::string& name) {
	m_pause_music = false;
	if (current_music_name != name) {
		current_music_name = name;
		Mix_HaltMusic();
		Mix_PlayMusic(get_music(name), -1);
	}
	else if (Mix_PlayingMusic() == 0) {
		Mix_ResumeMusic();
	}
}

void SoundManager::pause_music() {
	if (Mix_PlayingMusic() == 0) {
		return;
	}
	Mix_PauseMusic();
}

void SoundManager::play_chunk(const std::string& name, int loops) {
	const auto& mix_chunk = get_chunk(name);
	Mix_VolumeChunk(mix_chunk, m_volume_sound);
	Mix_PlayChannel(-1, mix_chunk, loops);
}

const int& SoundManager::get_volume_music() { return m_volume_music; }
void SoundManager::set_volume_music(int volume) { 
	m_volume_music = volume; 
	Mix_VolumeMusic(m_volume_music);
}

const int& SoundManager::get_volume_sound() { return m_volume_sound; }
void SoundManager::set_volume_sound(int volume) { m_volume_sound = volume; }
	
Mix_Music* SoundManager::get_music(const std::string& name) {
	auto it = m_mix_musics.find(name);
	if (it != m_mix_musics.end()) {
		return (it->second.get());
	}
	throw std::runtime_error("Undefined music");
}

Mix_Chunk* SoundManager::get_chunk(const std::string& name) {
	auto it = m_mix_chunks.find(name);
	if (it != m_mix_chunks.end()) {
		return (it->second.get());
	}
	throw std::runtime_error("Undefined chunk");
}

void SoundManager::parse(const ReaderData* data) {
	std::string filename;
	if (!data->get("filename", filename)) {
		throw std::runtime_error("Missing filename chunk/music");
	}
	
	std::string type;
	if (!data->get("type", type)) {
		throw std::runtime_error("Unknown type chunk/music");
	}
	
	if (from_string_to_type_sound(type) == TypeSound::MUSIC) {
		MixMusicPtr mix_music = MixMusicCreator::from_file(data->m_parent_path + filename);
		if (!mix_music) {
			std::ostringstream msg;
			msg << "Couldn't load mix_music " << filename << " :" << Mix_GetError();
			throw std::runtime_error(msg.str());
		}
		else {
			m_mix_musics[filename] = std::move(mix_music);
		}
	}
	else {
		auto mix_chunk = MixChunkCreator::from_file(data->m_parent_path + filename);
		if (!mix_chunk) {
			std::ostringstream msg;
			msg << "Couldn't load mix_chunk " << filename << " :" << Mix_GetError();
			throw std::runtime_error(msg.str());
		}
		else {
			m_mix_chunks[filename] = std::move(mix_chunk);
		}
	}
}

TypeSound from_string_to_type_sound(const std::string& name) {
	if (name == "music") return TypeSound::MUSIC;
	if (name == "chunk") return TypeSound::CHUNK;
	throw std::runtime_error("Unknown type of sound");
	return TypeSound::MUSIC;
}