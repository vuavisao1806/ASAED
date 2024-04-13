#ifndef HEADER_ASAED_VIDEO_TTF_SURFACE_MANAGER_HPP
#define HEADER_ASAED_VIDEO_TTF_SURFACE_MANAGER_HPP

#include <map>
#include <tuple>
#include <string>

#include "util/currenton.hpp"
#include "video/ttf_surface_ptr.hpp"

class TTFFont;

class TTFSurfaceManager final : public Currenton<TTFSurfaceManager> {
private:
	using Key = std::tuple<void*, std::string>;
	std::map<Key, TTFSurfacePtr> m_cache;

public:
	TTFSurfaceManager();
	~TTFSurfaceManager() override;

private:
	TTFSurfaceManager(const TTFSurfaceManager&) = delete;
	TTFSurfaceManager& operator=(const TTFSurfaceManager&) = delete;

public:
	TTFSurfacePtr create_surface(const TTFFont& font, const std::string& text);
	int get_surface_width(const TTFFont& font, const std::string& text);
};

#endif