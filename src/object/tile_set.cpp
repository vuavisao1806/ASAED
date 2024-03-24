#include "object/tile_set.hpp"

#include "object/tile_parser.hpp"
#include "util/log.hpp"

TileSet::TileSet() :
	m_tiles(1)
{}

TileSet::~TileSet() {
	m_tiles.clear();
}

std::unique_ptr<TileSet> TileSet::from_file(const std::string& filename) {
	std::unique_ptr tileset = std::make_unique<TileSet>();
	
	TileSetParser parser(*tileset.get(), filename);
	parser.parse();

	return tileset;
}

void TileSet::add_tile(uint32_t id, std::unique_ptr<Tile> tile) {
	if (id >= static_cast<uint32_t>(m_tiles.size())) {
		m_tiles.resize(id + 1);
	}

	if (m_tiles[id]) {
		log_warning << "Tile id " << id << " redefined\n";
	}
	else {
		m_tiles[id] = std::move(tile);
	}
}
const Tile& TileSet::get(const uint32_t id) const {
	if (id >= static_cast<uint32_t>(m_tiles.size())) {
		log_warning << "Tile id " << id << " is invalid\n";
		return *m_tiles[id].get();
	}
	else {
		Tile* tile = m_tiles[id].get();
		if (tile) {
			return *tile;
		}
		else {
			log_warning << "Tile id " << id << " is invalid\n";
			return *m_tiles[0].get();
		}
	}
}