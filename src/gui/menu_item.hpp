#ifndef HEADER_ASAED_GUI_MENU_ITEM_HPP
#define HEADER_ASAED_GUI_MENU_ITEM_HPP

#include "gui/menu.hpp"
#include "video/ttf_font_ptr.hpp"

#include <string>

class MenuItem {
private:
	int m_id;
	std::string m_text;
	TTFFontPtr m_font;

public:
	MenuItem(const std::string text, int id = -1);
	virtual ~MenuItem();

private:
	MenuItem(const MenuItem&) = delete;
	MenuItem& operator=(const MenuItem&) = delete;

public:
	/** draw the menu item */
	virtual void draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool is_active);

	/** return true when the menu item has no action (like labels and horizontal lines) */
	virtual bool skippable() const;

	/** return the distance between the items above and below */
	virtual float get_distance() const;
	virtual int get_width() const;
	virtual int get_height() const;

	virtual Color get_color() const;

	/** process the menu action */
	virtual void process_action(const MenuAction& action);

	/** process the given event */
	virtual void process_event(const SDL_Event& event);

public:
	int get_id() const;

	void set_text(const std::string& text);
	const std::string& get_text() const;

	void set_font(const TTFFontPtr font);
	const TTFFontPtr& get_font() const;

	
};

#endif