#ifndef HEADER_ASAED_OBJECT_TILE_HPP
#define HEADER_ASAED_OBJECT_TILE_HPP

#include <vector>
#include <string>
#include <stdint.h>

#include <math/vector.hpp>
#include "video/surface_ptr.hpp"

class Canvas;

/** Tile is the lowest level (but directly manages surfaces and some information for flexible request) */

class Tile final {
public:
	enum {
		/** to add tile attributes */
		SOLID           = 0x0001,
		UNSOLID         = 0x0002
	};

private:
	std::vector<SurfacePtr> m_images;
	uint32_t m_attributes;

	float m_fps;
	// maybe I don't need (will remove)
	std::string m_name;

public:
	Tile();
	Tile(const std::vector<SurfacePtr>& images, uint32_t attributes, float fps, const std::string& name = "");

private:
	Tile(const Tile&) = delete;
	Tile& operator=(const Tile&) = delete;

public:
	void draw(Canvas& canvas, const Vector& pos, int layer) const;

	uint32_t get_attributes() const;
	bool is_solid() const;
};

#endif