#include "gui/menu_storage.hpp"


#include "asaed/menu/main_menu.hpp"
#include "asaed/menu/options_menu.hpp"
#include "asaed/menu/game_menu.hpp"
#include "gui/menu.hpp"
#include "util/log.hpp"

MenuStorage::MenuStorage()
{}

MenuStorage::~MenuStorage()
{}

std::unique_ptr<Menu> MenuStorage::create(MenuID menu_id) {
	switch (menu_id) {
		case MAIN_MENU:
			return std::make_unique<MainMenu>();
			break;
		case OPTIONS_MENU:
			return std::make_unique<OptionsMenu>();
			break;
		case GAME_MENU:
			return std::make_unique<GameMenu>();
			break;
	
	default:
		log_warning << "Unknown type menu provided by MenuId: " << menu_id << '\n';
		assert(false);
		break;
	}
	return std::make_unique<MainMenu>();
}