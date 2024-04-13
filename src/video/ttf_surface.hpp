#ifndef HEADER_ASAED_VIDEO_TTF_SURFACE_HPP
#define HEADER_ASAED_VIDEO_TTF_SURFACE_HPP

#include "math/vector.hpp"
#include "video/surface_ptr.hpp"
#include "video/ttf_surface_ptr.hpp"

class TTFFont;

class TTFSurface final {
private:
	SurfacePtr m_surface;
	Vector m_offset;

public:
	TTFSurface(const SurfacePtr& surface, const Vector& offset);

private:
	TTFSurface(const TTFSurface&) = delete;
	TTFSurface& operator=(const TTFSurface&) = delete;

public:
	static TTFSurfacePtr create(const TTFFont& font, const std::string& text);

public:
	SurfacePtr get_surface();
	Vector get_offset() const;

	int get_width() const;
	int get_height() const;
};

#endif