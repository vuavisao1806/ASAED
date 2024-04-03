#include "weapon/moving_tile/moving_tile_set.hpp"

#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "util/log.hpp"
#include "weapon/moving_tile/moving_tile.hpp"
#include "weapon/moving_tile/moving_tile_analysis.hpp"


MovingTileSet::MovingTileSet() :
	m_filename("data/images/weapon/movingtile-set.json"),
	m_movingtiles(1)
{
	parse();
}

MovingTileSet::~MovingTileSet() {
	m_movingtiles.clear();
}

void MovingTileSet::add_movingtile(uint32_t id, std::unique_ptr<MovingTile> movingtile) {
	if (id >= static_cast<uint32_t>(m_movingtiles.size())) {
		m_movingtiles.resize(id + 1);
	}

	if (m_movingtiles[id]) {
		log_warning << "MovingTile id " << id << " redefined\n";
	}
	else {
		m_movingtiles[id] = std::move(movingtile);
	}
}

const MovingTile& MovingTileSet::get(const uint32_t id) const {
	if (id >= static_cast<uint32_t>(m_movingtiles.size())) {
		log_warning << "MovingTile id " << id << " is invalid\n";
		return *m_movingtiles[0].get();
	}
	else {
		MovingTile* movingtile = m_movingtiles[id].get();
		if (movingtile) {
			return *movingtile;
		}
		else {
			log_warning << "MovingTile id " << id << " is invalid\n";
			return *m_movingtiles[0].get();
		}
	}
}

void MovingTileSet::parse() {
	ReaderMachine reader(m_filename);
	
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse_movingtile(reader.get_data(i));
	}
}

void MovingTileSet::parse_movingtile(const ReaderData* data) {
	uint32_t id;

	if (!data->get("id", id)) {
		throw std::runtime_error("Missing movingtile id");
	}

	auto movingtile = MovingTileAnalysis::from_file(data);
	add_movingtile(id, std::move(movingtile));
}