#include "gui/item_goto.hpp"

#include <assert.h>
#include "gui/menu_manager.hpp"

ItemGoto::ItemGoto(const std::string text, int target_menu, int id) :
	MenuItem(text, id),
	m_target_menu(target_menu)
{}

void ItemGoto::process_action(const MenuAction& action) {
	if (action == MenuAction::HIT) {
		assert(m_target_menu != 0);
		MenuManager::current()->push_menu(m_target_menu);
	}
}