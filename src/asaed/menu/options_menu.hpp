#ifndef HEADER_ASAED_ASAED_MENU_OPTIONS_MENU_HPP
#define HEADER_ASAED_ASAED_MENU_OPTIONS_MENU_HPP

#include "gui/menu.hpp"

class OptionsMenu final : public Menu {
private:
	enum MenuID {
		MENU_ID_MUSIC,
		MENU_ID_SOUND
	};

	struct StringOption {
		std::vector<std::string> list;
		int next;
		StringOption() :
			list(),
			next(0)
		{}
	};

	StringOption m_music_volumes;
	StringOption m_sound_volumes;

public:
	OptionsMenu();
	~OptionsMenu() override;

private:
	OptionsMenu(const OptionsMenu&) = delete;
	OptionsMenu& operator=(const OptionsMenu&) = delete;

private:
	void insert_label(const std::string& text);
	void add_sound_volume();
	void add_music_volume();

	void menu_action(MenuItem& item) override;

	static bool less_than_volume(const std::string& lhs, const std::string& rhs);
};

#endif