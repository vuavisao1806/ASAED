#include "video/ttf_surface_manager.hpp"

#include "video/ttf_surface.hpp"
#include "video/ttf_font.hpp"


TTFSurfaceManager::TTFSurfaceManager() :
	m_cache()
{}

TTFSurfaceManager::~TTFSurfaceManager()
{
	m_cache.clear();
}

TTFSurfacePtr TTFSurfaceManager::create_surface(const TTFFont& font, const std::string& text) {
	auto key = Key(font.get_ttf_font(), text);
	auto it = m_cache.find(key);
	if (it == m_cache.end()) {
		TTFSurfacePtr ttf_surface = TTFSurface::create(font, text);
		m_cache[key] = ttf_surface;
		return ttf_surface;
	}
	return it->second;
}

int TTFSurfaceManager::get_surface_width(const TTFFont& font, const std::string& text) {
	auto key = Key(font.get_ttf_font(), text);
	auto it = m_cache.find(key);
	if (it == m_cache.end()) {
		// Anyway! If something doesn't exist, I won't care
		// return -1;
		// No I care, save everything, but maybe my memory will not happy
		TTFSurfacePtr ttf_surface = create_surface(font, text);
		return ttf_surface->get_width();
	}
	return it->second->get_width();
}