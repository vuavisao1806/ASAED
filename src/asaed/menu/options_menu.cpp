#include "asaed/menu/options_menu.hpp"

#include "audio/sound_manager.hpp"
#include "gui/item_goto.hpp"
#include "gui/item_string_select.hpp"
#include "gui/menu_item.hpp"
#include "gui/menu_storage.hpp"
#include "gui/menu_manager.hpp"


OptionsMenu::OptionsMenu() :
	m_music_volumes(),
	m_sound_volumes()
{
	insert_label("Audio");
	add_music_volume();
	add_sound_volume();
	add_horizontal_line();
	add_back("Back");
}

OptionsMenu::~OptionsMenu()
{}

void OptionsMenu::insert_label(const std::string& text) {
	add_label(text);
	add_horizontal_line();
}

void OptionsMenu::add_sound_volume() {
	m_sound_volumes.list = { "0%",
	                        "10%",
	                        "20%",
	                        "30%",
	                        "40%",
	                        "50%",
	                        "60%",
	                        "70%",
	                        "80%",
	                        "90%",
	                        "100%"};
	
	m_sound_volumes.next = SoundManager::current()->get_volume_sound() / 10;
	add_string_select(MENU_ID_SOUND, "Sound Volume", &m_sound_volumes.next, m_sound_volumes.list);
}

void OptionsMenu::add_music_volume() {
	m_music_volumes.list = { "0%",
	                       "10%",
	                       "20%",
	                       "30%",
	                       "40%",
	                       "50%",
	                       "60%",
	                       "70%",
	                       "80%",
	                       "90%",
	                       "100%"};
	
	m_music_volumes.next = SoundManager::current()->get_volume_music() / 10;
	add_string_select(MENU_ID_MUSIC, "Music Volume", &m_music_volumes.next, m_music_volumes.list);
}

bool OptionsMenu::less_than_volume(const std::string& lhs, const std::string& rhs) {
	int lhs_volume, rhs_volume;
	if (sscanf(lhs.c_str(), "%i", &lhs_volume) == 1 && sscanf(rhs.c_str(), "%i", &rhs_volume) == 1) {
		return lhs_volume < rhs_volume;
	}
	return false;
}


void OptionsMenu::menu_action(MenuItem& item) {
	int current_volume;
	switch (item.get_id()) {
		case MENU_ID_MUSIC:
			if (sscanf(m_music_volumes.list[m_music_volumes.next].c_str(), "%i", &current_volume) == 1) {
				SoundManager::current()->set_volume_music(current_volume);
			}
			break;
	
		case MENU_ID_SOUND:
			if (sscanf(m_music_volumes.list[m_music_volumes.next].c_str(), "%i", &current_volume) == 1) {
				SoundManager::current()->set_volume_music(current_volume);
			}
			break;
	
		default:
			break;
	}
}