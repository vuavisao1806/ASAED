#include "object/tile_manager.hpp"

#include "object/tile_set.hpp"

TileManager::TileManager() :
	m_tilesets()
{}

TileManager::~TileManager() {
	m_tilesets.clear();
}


TileSet* TileManager::get_tileset(const std::string& filename) {
	auto it = m_tilesets.find(filename);
	if (it != m_tilesets.end()) {
		return it->second.get();
	}
	else {
		auto tileset = TileSet::from_file(filename);
		m_tilesets[filename] = std::move(tileset);
		return m_tilesets[filename].get();
	}
}