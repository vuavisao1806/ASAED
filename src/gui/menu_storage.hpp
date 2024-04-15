#ifndef HEADER_ASAED_GUI_MENU_STORAGE_HPP
#define HEADER_ASAED_GUI_MENU_STORAGE_HPP

#include <memory>

#include "util/currenton.hpp"

class Menu;

class MenuStorage final : public Currenton<MenuStorage> {
public:
	enum MenuID {
		NO_MENU,
		MAIN_MENU,
		OPTIONS_MENU,
		GAME_MENU
	};

public:
	MenuStorage();
	~MenuStorage() override;

private:
	MenuStorage(const MenuStorage&) = delete;
	MenuStorage& operator=(const MenuStorage&) = delete;

public:
	std::unique_ptr<Menu> create(MenuID menu_id);
};

#endif