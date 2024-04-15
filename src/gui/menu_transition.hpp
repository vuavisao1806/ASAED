#ifndef HEADER_ASAED_GUI_MENU_TRANSITION_HPP
#define HEADER_ASAED_GUI_MENU_TRANSITION_HPP

#include "math/rectf.hpp"

class DrawingContext;

class MenuTransition final {
private:
	Rectf m_from_rect;
	Rectf m_to_rect;

	float m_effect_progress;
	float m_effect_start_time;
	bool m_is_active;

public:
	MenuTransition();
	~MenuTransition();

	void start(const Rectf& from_rect, const Rectf& to_rect);
	void update();
	void draw(DrawingContext& drawing_context);

	void set(const Rectf& rect);
	bool is_active() const;
};

#endif