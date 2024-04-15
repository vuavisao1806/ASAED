#include "gui/item_back.hpp"

#include "asaed/resources.hpp"
#include "gui/colorscheme.hpp"
#include "gui/menu_manager.hpp"
#include "video/surface.hpp"

ItemBack::ItemBack(const std::string& text, int id) :
	MenuItem(text, id)
{}


void ItemBack::draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool is_active) {
	float text_width = Resources::normal_font->get_text_width(get_text());
	drawing_context.get_canvas().draw_text(Resources::normal_font, get_text(),
	                                       Vector(position.x + static_cast<float>(menu_width) / 2.0f, 
	                                              position.y - static_cast<float>(static_cast<int>(Resources::normal_font->get_height() / 2.0f))),
	                                       ALIGN_CENTER, LAYER_GUI, (is_active ? ColorScheme::Menu::active_color : get_color()));
	drawing_context.get_canvas().draw_surface(Resources::back,
	                                          Vector(position.x + static_cast<float>(menu_width) / 2.0f + text_width / 2.0f + 16.0f, position.y - 8.0f),
	                                          LAYER_GUI);
}
	
int ItemBack::get_width() const { return static_cast<int>(Resources::normal_font->get_text_width(get_text())) + 32 + Resources::back->get_width(); }

void ItemBack::process_action(const MenuAction& action) {
	if (action == MenuAction::HIT) {
		if (MenuManager::current()->current_menu()->on_back_action()) {
			MenuManager::current()->pop_menu();
		}
	}
}