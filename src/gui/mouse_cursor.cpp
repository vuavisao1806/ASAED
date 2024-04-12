#include "gui/mouse_cursor.hpp"

#include "sprite/sprite.hpp"
#include "video/drawing_context.hpp"
#include "video/layer.hpp"
#include "video/video_system.hpp"

MouseCursor::MouseCursor(SpritePtr sprite) :
	m_state(MouseCursorState::NORMAL),
	m_sprite(std::move(sprite))
{}

void MouseCursor::draw(DrawingContext& drawing_context) {
	int x, y;
	uint32_t is_pressed = SDL_GetMouseState(&x, &y);
	
	if (is_pressed & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		apply_state(MouseCursorState::CLICK);
	}
	else {
		apply_state(MouseCursorState::NORMAL);
	}

	Vector mouse_pos = VideoSystem::current()->get_viewport().to_logical(x, y);
	
	Vector middle_position = mouse_pos - (Vector(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_width()) / 2.0f);
	m_sprite->draw(drawing_context.get_canvas(), middle_position, LAYER_GUI + 10);
}

void MouseCursor::apply_state(MouseCursorState state) { 
	if (m_state == state) {
		return;
	}
	m_state = state;
	switch (m_state) {
		case MouseCursorState::NORMAL:
			m_sprite->set_action("normal");
			break;
		case MouseCursorState::CLICK:
			m_sprite->set_action("click");
			break;
		default:
			break;
	}
}

MouseCursorState MouseCursor::get_state() const { return m_state; }
