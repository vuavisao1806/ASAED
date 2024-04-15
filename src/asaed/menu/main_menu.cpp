#include "asaed/menu/main_menu.hpp"

#include "audio/sound_manager.hpp"
#include "asaed/game_manager.hpp"
#include "asaed/screen_manager.hpp"
#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_storage.hpp"
#include "video/video_system.hpp"
#include "video/viewport.hpp"

MainMenu::MainMenu() {
	add_label("ASAED");
	add_horizontal_line();
	add_entry(MENU_ID_START, "Start Game");
	add_submenu("Options", MenuStorage::OPTIONS_MENU);
	add_entry(MENU_ID_QUIT, "Quit");
}

void MainMenu::menu_action(MenuItem& item) {
	switch (item.get_id()) {
		case MENU_ID_START:
			MenuManager::current()->clear_menu_stack();
			GameManager::current()->start_level("level1");
			break;
		
		case MENU_ID_QUIT:
			MenuManager::current()->clear_menu_stack();
			ScreenManager::current()->quit();
			break;
		
		default:
			break;
	}
}