#include "gui/item_horizontal_line.hpp"

#include "gui/colorscheme.hpp"

ItemHorizontalLine::ItemHorizontalLine() :
	MenuItem("")
{}

void ItemHorizontalLine::draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool /* is_active */) {
	// Draw a horizontal line with a little 3D effect
	// Let me see! Let me see
	drawing_context.get_canvas().draw_filled_rect(Rectf(Vector(position.x, position.y - 6.0f),
	                                              Sizef(static_cast<float>(menu_width), 4.0f)),
	                                              ColorScheme::Menu::horizontal_line_color, LAYER_GUI);
	drawing_context.get_canvas().draw_filled_rect(Rectf(Vector(position.x, position.y - 6.0f),
	                                              Sizef(static_cast<float>(menu_width), 2.0f)),
	                                              ColorScheme::Menu::default_color, LAYER_GUI);
	
}

bool ItemHorizontalLine::skippable() const { return true;}

int ItemHorizontalLine::get_width() const { return 0; }