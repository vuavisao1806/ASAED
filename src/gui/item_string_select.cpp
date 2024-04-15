#include "gui/item_string_select.hpp"

#include "asaed/resources.hpp"
#include "gui/colorscheme.hpp"
#include "gui/menu_manager.hpp"
#include "video/drawing_context.hpp"
#include "video/surface.hpp"

ItemStringSelect::ItemStringSelect(const std::string& text, std::vector<std::string> items, int* selected, int id) :
	MenuItem(text, id),
	m_items(std::move(items)),
	m_selected(std::move(selected)),
	m_callback(),
	m_width(re_calculate_width())
{}

ItemStringSelect::~ItemStringSelect()
{}

void ItemStringSelect::draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool is_active) {
	float offset = static_cast<float>(Resources::arrow_left->get_width());
	float self_width = Resources::normal_font->get_text_width(m_items[*m_selected]);

	drawing_context.get_canvas().draw_text(Resources::normal_font, get_text(),
	                                       Vector(position.x + 16.0f,
	                                              position.y - Resources::normal_font->get_height() / 2.0f),
	                                       ALIGN_LEFT, LAYER_GUI, (is_active ? ColorScheme::Menu::active_color : get_color()));

	drawing_context.get_canvas().draw_surface(Resources::arrow_left,
	                                          Vector(position.x + static_cast<float>(menu_width) - self_width - 2.0f * offset - 8.0f,
	                                                 position.y - 8.0f),
	                                          LAYER_GUI);

	drawing_context.get_canvas().draw_surface(Resources::arrow_right,
	                                          Vector(position.x + static_cast<float>(menu_width) - offset - 8.0f,
	                                                 position.y - 8.0f),
	                                          LAYER_GUI);
	
	drawing_context.get_canvas().draw_text(Resources::normal_font, m_items[*m_selected],
	                                       Vector(position.x + static_cast<float>(menu_width) - offset - 8.0f,
	                                              position.y - Resources::normal_font->get_height() / 2.0f),
	                                       ALIGN_RIGHT, LAYER_GUI, (is_active ? ColorScheme::Menu::active_color : get_color()));
}

int ItemStringSelect::get_width() const { return static_cast<int>(m_width); }

void ItemStringSelect::process_action(const MenuAction& action) {
	switch (action) {
		case MenuAction::LEFT:
			*m_selected = (*m_selected - 1 + static_cast<int>(m_items.size())) % static_cast<int>(m_items.size());
			MenuManager::current()->current_menu()->menu_action(*this);
			if (m_callback) {
				m_callback(*m_selected);
			}
			break;

		case MenuAction::RIGHT:
		case MenuAction::HIT:
			*m_selected = (*m_selected + 1) % static_cast<int>(m_items.size());
			MenuManager::current()->current_menu()->menu_action(*this);
			if (m_callback) {
				m_callback(*m_selected);
			}
			break;

		default:
			break;
	}

}

void ItemStringSelect::set_callback(const std::function<void(int)>& callback) { m_callback = callback; }

float ItemStringSelect::re_calculate_width() const {
	float max_item_width = 0;
	for (const auto& item : m_items) {
		max_item_width = std::max(max_item_width, Resources::normal_font->get_text_width(item));
	}
	return Resources::normal_font->get_text_width(get_text()) + max_item_width + 64.0f;
}