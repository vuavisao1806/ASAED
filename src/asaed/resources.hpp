#ifndef HEADER_ASAED_ASAED_RESOURCES_HPP
#define HEADER_ASAED_ASAED_RESOURCES_HPP

#include <memory>

#include "video/ttf_font_ptr.hpp"
#include "video/surface_ptr.hpp"

class MouseCursor;

class Resources final {
public:
	static std::unique_ptr<MouseCursor> m_mouse_cursor;

	static TTFFontPtr normal_font;
	static TTFFontPtr small_font;
	static TTFFontPtr big_font;

	static SurfacePtr arrow_left;
	static SurfacePtr arrow_right;
	static SurfacePtr back;
public:
	Resources();
	~Resources();

public:
	void load();
	void unload();
};

#endif