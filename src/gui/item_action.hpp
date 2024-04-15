#ifndef HEADER_ASAED_ITEM_ACTION_HPP
#define HEADER_ASAED_ITEM_ACTION_HPP

#include "gui/menu_item.hpp"

class ItemAction final : public MenuItem {
private:
	std::function<void()> m_callback;

private:
	ItemAction(const ItemAction&) = delete;
	ItemAction& operator=(const ItemAction&) = delete;

public:
	ItemAction(const std::string& text, int id = -1, std::function<void()> callback = {});

public:
	virtual void process_action(const MenuAction& action) override;
};

#endif