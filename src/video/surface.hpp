#ifndef HEADER_ASAED_context_HPP
#define HEADER_ASAED_context_HPP

#include <string>
#include <optional>

#include "math/rect.hpp"
#include "video/flip.hpp"
#include "video/surface_ptr.hpp"
#include "video/texture_ptr.hpp"

/**
 * This class is a rectangular image. It basically holds a TexturePtr
 * and some information that specifies an area of the texture
 * One reason to use this class is when making texture we need SDL_CreateTextureFromSurface()
*/

class Surface final {
private:
	const TexturePtr m_texture;
	const Rect m_region;
	// It's the mask of Flip, (it doesn't flip here)
	const Flip m_flip;
	const std::string m_filename;

public:
	Surface(const TexturePtr& texture, const Flip flip, const std::string& filename);
	Surface(const TexturePtr& texture, const Rect& region, const Flip flip, const std::string& filename);

public:
	~Surface();

private:
	Surface& operator=(const Surface&) = delete;

public:
	static SurfacePtr from_texture(const TexturePtr& texture);
	static SurfacePtr from_file(const std::string& filename, const std::optional<Rect>& rect);

public:
	SurfacePtr region(const Rect& rect) const;
	/** It clones and flips */
	SurfacePtr clone_flip(const Flip flip = NO_FLIP) const;

	TexturePtr get_texture() const;
	Rect get_region() const;
	int get_width() const;
	int get_height() const;
	Flip get_flip() const;
	std::string get_filename() const;
};

#endif