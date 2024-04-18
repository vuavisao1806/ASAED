#include "gui/item_label.hpp"

#include "asaed/resources.hpp"
#include "gui/colorscheme.hpp"

ItemLabel::ItemLabel(const std::string& text) :
	MenuItem(text)
{}

void ItemLabel::draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool /* is_active */) {
	drawing_context.get_canvas().draw_text(Resources::big_font, get_text(),
	                                       Vector(position.x + static_cast<float>(menu_width) / 2.0f,
	                                              position.y - Resources::big_font->get_height() / 2.0f),
	                                       ALIGN_CENTER, LAYER_GUI, get_color());
}

bool ItemLabel::skippable() const { return true; }

int ItemLabel::get_width() const { return static_cast<int>(Resources::big_font->get_text_width(get_text())) + 16; }
Color ItemLabel::get_color() const { return ColorScheme::Menu::label_color; }