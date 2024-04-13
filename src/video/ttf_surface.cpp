#include "video/ttf_surface.hpp"

#include "util/log.hpp"
#include "video/ttf_font.hpp"
#include "video/surface.hpp"
#include "video/surface_creator.hpp"
#include "video/video_system.hpp"

TTFSurface::TTFSurface(const SurfacePtr& surface, const Vector& offset) :
	m_surface(surface),
	m_offset(offset)
{}

TTFSurfacePtr TTFSurface::create(const TTFFont& font, const std::string& text) {
	SDLSurfacePtr text_surface(TTF_RenderUTF8_Blended(font.get_ttf_font(), text.c_str(), SDL_Color{0xff, 0xff, 0xff, 0xff}));
	if (!text_surface) {
		log_warning << "Couldn't create text '" << text << "' :" << TTF_GetError() << '\n';
		// hmm maybe I should throw or ...
		return std::make_shared<TTFSurface>(SurfacePtr(), Vector(0.0f, 0.0f));
	}
	SurfacePtr result = Surface::from_texture(VideoSystem::current()->new_texture(*text_surface));
	return std::make_shared<TTFSurface>(result, Vector(0.0f, 0.0f));
}

SurfacePtr TTFSurface::get_surface() { return m_surface; }
Vector TTFSurface::get_offset() const { return m_offset; }

int TTFSurface::get_width() const {
	if (m_surface) {
		return m_surface->get_width();
	}
	return 0;
}
int TTFSurface::get_height() const {
	if (m_surface) {
		return m_surface->get_height();
	}
	return 0;
}