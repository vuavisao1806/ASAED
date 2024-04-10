#include "asaed/level_manager.hpp"

#include "asaed/room_analysis.hpp"
#include "asaed/level_data.hpp"
#include "object/tile_set.hpp"
#include "object/tile_manager.hpp"
#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"

LevelManager::LevelManager() :
	m_filename("data/images/level/level.json"),
	m_levels()
{
	ReaderMachine reader(m_filename);
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse_level(reader.get_data(i));
	}
}

LevelManager::~LevelManager() {
	m_levels.clear();
}

const LevelData& LevelManager::get(const std::string& filename) const {
	auto it = m_levels.find(filename);
	if (it == m_levels.end()) {
		throw std::runtime_error("Undefined level");
	}
	return *it->second.get();
}

#include "util/log.hpp"
void LevelManager::parse_level(const ReaderData* data) {
	auto level_data = std::make_unique<LevelData>();
	std::string level_name;
	if (!data->get("level", level_name)) {
		throw std::runtime_error("Unknown level!!");
	}

	std::string next_level_name;
	data->get("next-level", next_level_name);
	level_data->set_next_level(next_level_name);

	std::string file_tileset;
	if (!data->get("tileset", file_tileset)) {
		throw std::runtime_error("Missing tileset in level!!");
	}
	level_data->m_tileset = TileManager::current()->get_tileset(data->m_parent_path + file_tileset);

	std::vector<std::string> rooms;
	data->get("room", rooms);
	for (const auto& filename : rooms) {
		ReaderMachine reader(data->m_parent_path + filename);
		for (size_t i = 0; i < reader.get_size(); ++ i) {
			const ReaderData* data_room = reader.get_data(i);
			int id;
			if (!data_room->get("id", id)) {
				throw std::runtime_error("Missing id in room!!");
			}
			auto room = RoomAnalysis::fromfile(level_data->m_tileset, data_room);
			level_data->add_room(id, std::move(room));
		}
	}
	m_levels[level_name] = std::move(level_data);
}