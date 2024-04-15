#ifndef HEADER_ASAED_GUI_ITEM_GUI_HPP
#define HEADER_ASAED_GUI_ITEM_GUI_HPP

#include "gui/menu_item.hpp"

class ItemGoto final : public MenuItem {
public:
	int m_target_menu;

public:
	ItemGoto(const std::string text, int target_menu, int id = -1);

private:
	ItemGoto(const ItemGoto&) = delete;
	ItemGoto& operator=(const ItemGoto&) = delete;

public:
	/** process the menu action */
	virtual void process_action(const MenuAction& action) override;
};

#endif