#ifndef HEADER_ASAED_ASAED_MENU_GAME_MENU_HPP
#define HEADER_ASAED_ASAED_MENU_GAME_MENU_HPP

#include "gui/menu.hpp"

#include <functional>

class GameMenu final : public Menu {
private:
	std::function<void()> quit_callback;

	enum GameMenuID {
		MENU_ID_CONTINUE,
		MENU_ID_QUIT
	};

public:
	GameMenu();

private:
	GameMenu(const GameMenu&) = delete;
	GameMenu& operator=(const GameMenu&) = delete;

public:
	void menu_action(MenuItem& item) override;

};

#endif