#ifndef HEADER_ASAED_OBJECT_TILE_MANAGER_HPP
#define HEADER_ASAED_OBJECT_TILE_MANAGER_HPP

#include <map>
#include <memory>
#include <string>

#include "util/currenton.hpp"

class TileSet;

/**
 * What is TileManager?
 * TileManager stores and manages all tilesets
 * Tileset manages its tiles
 * Tile is the lowest level (but directly manages surfaces and some information for flexible request)
*/
class TileManager final : public Currenton<TileManager> {
private:
	std::map<std::string, std::unique_ptr<TileSet>> m_tilesets;

public:
	TileManager();
	~TileManager() override;

public:
	TileSet* get_tileset(const std::string& filename);
};

#endif