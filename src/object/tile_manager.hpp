#ifndef HEADER_ASAED_OBJECT_TILE_MANAGER_HPP
#define HEADER_ASAED_OBJECT_TILE_MANAGER_HPP

#include <map>
#include <memory>
#include <string>

#include "util/currenton.hpp"


class TileSet;

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