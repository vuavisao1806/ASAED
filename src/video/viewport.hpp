#ifndef HEADER_ASAED_VIDEO_VIEWPORT_HPP
#define HEADER_ASAED_VIDEO_VIEWPORT_HPP

#include "math/rect.hpp"
#include "math/vector.hpp"

// to be honest, Viewport may only used in case window_size

class Viewport final {
private:
	Rect m_rect;
	Vector m_scale;

public:
	Viewport();
	Viewport(const Rect& rect, const Vector& scale);

public:

	/** the size of the viewport in window coordinates*/
	Rect get_rect() const;
	/** the amount by which the content of the viewport is scaled */
	Vector get_scale() const;

	/** the width of the resulting logical screen */
	int get_screen_width() const;
	/** the height of the resulting logical screen */
	int get_screen_height() const;
	/** the size of the resulting logical screen */
	Size get_screen_size() const;

	/** 
	 * converts window coordinates into logical screen coordinates 
	 * To be honest, I think I only use this function to help mouse_event 
	*/
	Vector to_logical(int physical_x, int physical_y) const;
};

#endif