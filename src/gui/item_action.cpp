#include "gui/item_action.hpp"


ItemAction::ItemAction(const std::string& text, int id, std::function<void()> callback) :
	MenuItem(text, id),
	m_callback(std::move(callback))
{}

void ItemAction::process_action(const MenuAction& action) {
	if (action == MenuAction::HIT) {
		if (m_callback) {
			m_callback();
		}
	}
}
