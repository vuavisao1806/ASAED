#include "gui/menu_item.hpp"

#include "asaed/resources.hpp"
#include "gui/colorscheme.hpp"

MenuItem::MenuItem(const std::string text, int id) :
	m_id(id),
	m_text(text),
	m_font(Resources::normal_font)
{}

MenuItem::~MenuItem()
{}

void MenuItem::draw(DrawingContext& drawing_context, const Vector& position, int menu_width, bool is_active) {
	drawing_context.get_canvas().draw_text(m_font, m_text, 
	                                       Vector(position.x + static_cast<float>(menu_width) / 2.0f, 
	                                              position.y - static_cast<float>(m_font->get_height()) / 2.0f),
	                                       ALIGN_CENTER, LAYER_GUI, (is_active ? ColorScheme::Menu::active_color : get_color()));
}

bool MenuItem::skippable() const { return false; }

float MenuItem::get_distance() const { return 0.0f; }
int MenuItem::get_width() const { return static_cast<int>(m_font->get_text_width(m_text)) + 16; /* options */ }
int MenuItem::get_height() const { return 24; /* options */ }

Color MenuItem::get_color() const { return ColorScheme::Menu::default_color; }

void MenuItem::process_action(const MenuAction& /* action */) {}

void MenuItem::process_event(const SDL_Event& /* event */) {}

int MenuItem::get_id() const { return m_id; }

void MenuItem::set_text(const std::string& text) { m_text = text; }
const std::string& MenuItem::get_text() const { return m_text; }

void MenuItem::set_font(const TTFFontPtr font) { m_font = font; }
const TTFFontPtr& MenuItem::get_font() const { return m_font; }