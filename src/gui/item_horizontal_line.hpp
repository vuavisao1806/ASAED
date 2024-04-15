#ifndef HEADER_ASAED_GUI_ITEM_HORIZONTAL_LINE_HPP
#define HEADER_ASAED_GUI_ITEM_HORIZONTAL_LINE_HPP

#include "gui/menu_item.hpp"

class ItemHorizontalLine final : public MenuItem {
public:
	ItemHorizontalLine();

private:
	ItemHorizontalLine(const ItemHorizontalLine&) = delete;
	ItemHorizontalLine& operator=(const ItemHorizontalLine&) = delete;

public:
	/** draw the menu item */
	virtual void draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool is_active) override;

	/** return true when the menu item has no action (like labels and horizontal lines) */
	virtual bool skippable() const override;

	virtual int get_width() const override;
};

#endif