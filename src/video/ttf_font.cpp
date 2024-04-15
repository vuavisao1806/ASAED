#include "video/ttf_font.hpp"

#include <assert.h>
#include <numeric>
#include <sstream>
#include <stdexcept>

#include "util/line_iterator.hpp"
#include "video/canvas.hpp"
#include "video/ttf_surface_manager.hpp"
#include "video/ttf_surface.hpp"


TTFFont::TTFFont(const std::string& filename, int size, float line_spacing) :
	m_font(),
	m_filename(filename),
	m_font_size(size),
	m_line_spacing(line_spacing)
{
	m_font = TTF_OpenFont(m_filename.c_str(), m_font_size);
	if (!m_font) {
		std::ostringstream msg;
		msg << "Couldn't load TTFont: '" << m_filename << "' : " << TTF_GetError();
		throw std::runtime_error(msg.str());
	}
}

TTFFont::~TTFFont() {
	TTF_CloseFont(m_font);
}

TTF_Font* TTFFont::get_ttf_font() const { return m_font;}
int TTFFont::get_font_size() const { return m_font_size; }
float TTFFont::get_line_spacing() const { return m_line_spacing; }
float TTFFont::get_height() const { return static_cast<float>(m_font_size) * m_line_spacing; }

float TTFFont::get_text_width(const std::string& text) const {
	if (text.empty()) {
		return 0.0f;
	}

	float max_width = 0.0f;
	LineIterator iter(text);
	while (iter.next()) {
		const std::string& line = iter.get();
		int line_width = TTFSurfaceManager::current()->get_surface_width(*this, line);
		assert(line_width >= 0.0f);
		max_width = std::max(max_width, static_cast<float>(line_width));
	}
	return max_width;
}

float TTFFont::get_text_height(const std::string& text) const {
	if (text.empty()) {
		return 0.0f;
	}
	return std::accumulate(text.begin(), text.end(), get_height(), [this] (float accumulator, const char c) {
		return accumulator += (c == '\n' ? get_height() : 0.0f);
	});
}

void TTFFont::draw_text(Canvas& canvas, const std::string& text, const Vector& pos, FontAlignment alignment, int layer, const Color& color) {
	float last_y = pos.y - (static_cast<float>(TTF_FontHeight(m_font)) - get_height()) / 2.0f;

	LineIterator iter(text);
	while (iter.next()) {
		const std::string& line = iter.get();
	
		if(!line.empty()) {
			TTFSurfacePtr ttf_surface = TTFSurfaceManager::current()->create_surface(*this, line);

			Vector new_pos(pos.x, last_y);
			if (alignment == ALIGN_CENTER) {
				new_pos.x -= static_cast<float>(ttf_surface->get_width()) / 2.0f;
			}
			else if (alignment == ALIGN_RIGHT) {
				new_pos.x -= static_cast<float>(ttf_surface->get_width());
			}

			canvas.draw_surface(ttf_surface->get_surface(), new_pos, 0.0f, color, layer);
		}
		last_y += get_height();
	}
}