#ifndef HEADER_ASAED_GUI_MENU_HPP
#define HEADER_ASAED_GUI_MENU_HPP

#include <SDL.h>
#include <functional>
#include <memory>
#include <vector>

#include "math/vector.hpp"
#include "gui/menu_action.hpp"
#include "video/color.hpp"
#include "video/drawing_context.hpp"

class ItemAction;
class ItemBack;
class ItemGoto;
class ItemHorizontalLine;
class ItemLabel;
class ItemStringSelect;

class MenuItem;

class Menu {
private:
	/** position of the menu (middle of the menu) */
	Vector m_position;
	float m_menu_width;
	float m_menu_height;

public:
	std::vector<std::unique_ptr<MenuItem>> m_items;

protected:
	int m_active_item;

public:
	Menu();
	virtual ~Menu();

private:
	Menu(const Menu&) = delete;
	Menu& operator=(const Menu&) = delete;

public:
	virtual void menu_action(MenuItem& item) = 0;
	/** return true if it should perform the back action, other else false */
	virtual bool on_back_action();

	void draw(DrawingContext& drawing_context);
	void draw_item(DrawingContext& drawing_context, int index, float position_y);

	virtual void process_event(const SDL_Event& event);
	virtual void process_action(const MenuAction& action);

	ItemHorizontalLine& add_horizontal_line();
	ItemLabel& add_label(const std::string& text);
	ItemAction& add_entry(int id, const std::string& text);
	ItemAction& add_entry(const std::string& text, const std::function<void()>& callback);
	ItemBack& add_back(const std::string& text, int id = -1);
	ItemGoto& add_submenu(const std::string& text, int submenu, int id = -1);
	ItemStringSelect& add_string_select(int id, const std::string& text, int* selected, const std::vector<std::string>& items);

	void clear();

	MenuItem& get_item(int index);

	MenuItem& get_item_by_id(int id);
	const MenuItem& get_item_by_id(int id) const;

	int get_active_item_id() const;
	void set_active_item(int id);

	Vector get_center_position() const;
	void set_center_position(float x, float y);

	float get_width() const;
	float get_height() const;

protected:
	MenuItem& add_item(std::unique_ptr<MenuItem> menu_item);
	MenuItem& add_item(std::unique_ptr<MenuItem> menu_item, int position);
	void delete_item(int position);

	void re_calculate_width();
	void re_calculate_height();

};


#endif