#include "video/viewport.hpp"

Viewport::Viewport() :
	m_rect(),
	m_scale(0.0f, 0.0f)
{}

Viewport::Viewport(const Rect& rect, const Vector& scale) :
	m_rect(rect),
	m_scale(scale)
{}

Rect Viewport::get_rect() const {
	return m_rect;
}

Vector Viewport::get_scale() const {
	return m_scale;
}

int Viewport::get_screen_width() const {
	return static_cast<int>(static_cast<float>(m_rect.get_width()) / m_scale.x);
}

int Viewport::get_screen_height() const {
	return static_cast<int>(static_cast<float>(m_rect.get_height()) / m_scale.y);
}

Size Viewport::get_screen_size() const {
	return Size(get_screen_width(), get_screen_height());
}

Vector Viewport::to_logical(int physical_x, int physical_y) const {
	return Vector(static_cast<float>(physical_x - m_rect.left) / m_scale.x,
	              static_cast<float>(physical_y - m_rect.top) / m_scale.y);
}