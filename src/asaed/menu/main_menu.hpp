#ifndef HEADER_ASAED_ASAED_MAIN_MENU_HPP
#define HEADER_ASAED_ASAED_MAIN_MENU_HPP

#include "gui/menu.hpp"

class MainMenu final : public Menu {
private:
	enum MainMenuID {
		MENU_ID_START,
		MENU_ID_CREDITS,
		MENU_ID_QUIT
	};

public:
	MainMenu();

private:
	MainMenu(const MainMenu&) = delete;
	MainMenu& operator=(const MainMenu&) = delete;

public:
	void menu_action(MenuItem& item) override;
};


#endif
