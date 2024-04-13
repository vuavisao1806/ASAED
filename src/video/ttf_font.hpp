#ifndef HEADER_ASAED_VIDEO_TTF_FONT_HPP
#define HEADER_ASAED_VIDEO_TTF_FONT_HPP

#include <SDL_ttf.h>
#include <string>

#include "video/color.hpp"
#include "math/vector.hpp"

enum FontAlignment {
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER
};

class Canvas;

class TTFFont final {
private:
	TTF_Font* m_font;
	std::string m_filename;
	int m_font_size;
	float m_line_spacing;

public:
	TTFFont(const std::string& filename, int size, float line_spacing = 1.0f);
	~TTFFont();

private:
	TTFFont(const TTFFont&) = delete;
	TTFFont& operator=(const TTFFont&) = delete;

public:
	TTF_Font* get_ttf_font() const;
	int get_font_size() const;
	float get_line_spacing() const;
	float get_height() const;

	float get_text_width(const std::string& text) const;
	float get_text_height(const std::string& text) const;

	void draw_text(Canvas& canvas, const std::string& text, const Vector& pos, FontAlignment alignment, int layer, const Color& color) const;
};

#endif