#include "gui/menu_transition.hpp"


#include "asaed/globals.hpp"
#include "gui/colorscheme.hpp"
#include "video/drawing_context.hpp"

MenuTransition::MenuTransition() :
	m_from_rect(),
	m_to_rect(),
	m_effect_progress(1.0f),
	m_effect_start_time(),
	m_is_active(false)
{}


MenuTransition::~MenuTransition()
{}

void MenuTransition::start(const Rectf& from_rect, const Rectf& to_rect) {
	m_from_rect = from_rect;
	m_to_rect = to_rect;

	m_effect_start_time = g_real_time;
	m_effect_progress = 0.0f;

	m_is_active = true;
}
void MenuTransition::update() {
	if (m_is_active) {
		m_effect_progress = (g_real_time - m_effect_progress) * 6.0f; // options;

		if (m_effect_progress > 1.0f) {
			m_effect_progress = 1.0f;
			m_is_active = false;
		}
	}
}

void MenuTransition::draw(DrawingContext& drawing_context) {
	const float& cur = m_effect_progress;

	Rectf rect = m_to_rect;
	if (m_is_active) {
		// zoom in and zoom out in same time
		rect = Rectf((m_to_rect.get_left() * cur) + (m_from_rect.get_left() * (1.0f - cur)),
		             (m_to_rect.get_top() * cur) + (m_from_rect.get_top() * (1.0f - cur)),
		             (m_to_rect.get_right() * cur) + (m_from_rect.get_right() * (1.0f - cur)),
		             (m_to_rect.get_bottom() * cur) + (m_from_rect.get_bottom() * (1.0f - cur)));

		// options
		drawing_context.get_canvas().draw_filled_rect(Rectf(rect.get_left() - 4.0f, rect.get_top() - 14.0f,
		                                                    rect.get_right() + 4.0f, rect.get_bottom() + 14.0f),
		                                              ColorScheme::Menu::back_color, LAYER_GUI - 10);
		drawing_context.get_canvas().draw_filled_rect(Rectf(rect.get_left(), rect.get_top() - 10.0f,
		                                                    rect.get_right(), rect.get_bottom() + 10.0f),
		                                              ColorScheme::Menu::front_color, LAYER_GUI - 10);
	}
}

void MenuTransition::set(const Rectf& rect) { m_from_rect = m_to_rect = rect; }
bool MenuTransition::is_active() const { return m_is_active; }