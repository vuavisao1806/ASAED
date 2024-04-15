#include "asaed/menu/game_menu.hpp"

#include "asaed/game_session.hpp"
#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_storage.hpp"

GameMenu::GameMenu() :
	quit_callback ([] {
		GameSession::current()->quit_game();
	})
{
	add_label("ASAED");
	add_horizontal_line();
	add_entry(MENU_ID_CONTINUE, "Continue");
	add_submenu("Options", MenuStorage::OPTIONS_MENU);
	add_horizontal_line();
	add_entry(MENU_ID_QUIT, "Quit");
}

void GameMenu::menu_action(MenuItem& item) {
	switch (item.get_id()) {
		case MENU_ID_CONTINUE:
			MenuManager::current()->clear_menu_stack();
			GameSession::current()->toggle_pause();
			break;

		case MENU_ID_QUIT:
			quit_callback();
			break;
		default:
			break;
	}
}