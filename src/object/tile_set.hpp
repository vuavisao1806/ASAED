#ifndef HEADER_ASAED_OBJECT_TILE_SET_HPP
#define HEADER_ASAED_OBJECT_TILE_SET_HPP

#include <memory>
#include <vector>
#include <string>

#include "object/tile.hpp"

class TileSet final {
private:
	std::vector<std::unique_ptr<Tile>> m_tiles;

public:
	TileSet();
	~TileSet();

public:
	static std::unique_ptr<TileSet> from_file(const std::string& filename);

private:
	TileSet(const TileSet&) = delete;
	TileSet& operator=(const TileSet&) = delete;

public:
	void add_tile(uint32_t id, std::unique_ptr<Tile> tile);
	const Tile& get(const uint32_t id) const;
};

#endif