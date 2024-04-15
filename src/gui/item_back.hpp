#ifndef HEADER_ASAED_GUI_ITEM_BACK_HPP
#define HEADER_ASAED_GUI_ITEM_BACK_HPP

#include "gui/menu_item.hpp"

class ItemBack final : public MenuItem {
public:
	ItemBack(const std::string& text, int id = -1);

private:
	ItemBack(const ItemBack&) = delete;
	ItemBack& operator=(const ItemBack&) = delete;

public:
	/** draw the menu item */
	virtual void draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool is_active) override;
	
	virtual int get_width() const override;

	/** process the menu action */
	virtual void process_action(const MenuAction& action) override;
};

#endif