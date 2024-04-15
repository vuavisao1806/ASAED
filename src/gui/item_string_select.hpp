#ifndef HEADER_ASAED_GUI_ITEM_STRING_SELECT_HPP
#define HEADER_ASAED_GUI_ITEM_STRING_SELECT_HPP

#include <functional>

#include "gui/menu_item.hpp"

class ItemStringSelect final : public MenuItem {
private:
	// list of values presentation by string
	std::vector<std::string> m_items;
	int* m_selected;
	std::function<void(int)> m_callback;
	int m_width;

public:
	ItemStringSelect(const std::string& text, std::vector<std::string> items, int* selected, int id = -1);
	~ItemStringSelect() override;

public:
	/** draw the menu item */
	virtual void draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool is_active);

	virtual int get_width() const override;

	/** process the menu action */
	virtual void process_action(const MenuAction& action) override;

	void set_callback(const std::function<void(int)>& callback);

private:
	float re_calculate_width() const;
};

#endif