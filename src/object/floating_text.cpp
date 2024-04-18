#include "object/floating_text.hpp"

#include "video/drawing_context.hpp"
#include "asaed/resources.hpp"
#include "gui/colorscheme.hpp"

namespace {
	const float DISPLAY_TIME = 0.5f;
	const float FADING_TIME  = 0.4f;
	const float MOVEMENT = 75.0f;
}

FloatingText::FloatingText(const Vector& position, const std::string& text):
	m_position(position),
	m_text(text),
	m_timer()
{
	m_timer.start(DISPLAY_TIME, false);
}

FloatingText::FloatingText(const Vector& position, int number):
	m_position(position),
	m_text(std::to_string(number)),
	m_timer()
{
	m_timer.start(DISPLAY_TIME, false);
}

void FloatingText::update(float dt_sec) {
	m_position.y -= MOVEMENT * dt_sec;
	if (m_timer.check()) {
		remove_me();
	}
}

void FloatingText::draw(DrawingContext& drawing_context) {
	float alpha;
	// Make an alpha animation when disappearing (It's cool!! I love it)
	if (m_timer.get_timeleft() < FADING_TIME) {
		alpha = m_timer.get_timeleft() * 255.0f / FADING_TIME;
	}
	else {
		alpha = 122.5f;
	}

	drawing_context.push_transform();
	drawing_context.set_alpha(alpha);

	drawing_context.get_canvas().draw_text(Resources::small_font, m_text, m_position, ALIGN_LEFT, LAYER_HUD, ColorScheme::FloatingText::text_color);

	drawing_context.pop_transform();
}