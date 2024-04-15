#ifndef HEADER_ASAED_ASAED_MOUSE_CURSOR_HPP
#define HEADER_ASAED_ASAED_MOUSE_CURSOR_HPP

#include "util/currenton.hpp"
#include "sprite/sprite_ptr.hpp"

class DrawingContext;

enum class MouseCursorState {
	NORMAL,
	CLICK,
	LINK,
	HIDE
};

class MouseCursor final : public Currenton<MouseCursor> {
private:
	MouseCursorState m_state;
	MouseCursorState m_applied_state;
	SpritePtr m_sprite;

public:
	MouseCursor(SpritePtr sprite);

private:
	MouseCursor(const MouseCursor&) = delete;
	MouseCursor& operator=(const MouseCursor&) = delete;

public:
	void draw(DrawingContext& drawing_context);
	void apply_state(MouseCursorState state);
	
	void set_state(MouseCursorState state);
	MouseCursorState get_state() const;
};

#endif