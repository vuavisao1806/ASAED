#ifndef HEADER_ASAED_GUI_ITEM_LABEL_HPP
#define HEADER_ASAED_GUI_ITEM_LABEL_HPP

#include "gui/menu_item.hpp"

class ItemLabel final : public MenuItem {
public:
	ItemLabel(const std::string& text);

private:
	ItemLabel(const ItemLabel&) = delete;
	ItemLabel& operator=(const ItemLabel&) = delete;

public:
	/** draw the menu item */
	virtual void draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool is_active) override;

	/** return true when the menu item has no action (like labels and horizontal lines) */
	virtual bool skippable() const override;

	virtual int get_width() const override;
	virtual Color get_color() const override;
};

#endif