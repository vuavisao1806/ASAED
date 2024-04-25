#include "asaed/level_data.hpp"

#include "asaed/level.hpp"
#include "asaed/level_manager.hpp"
#include "asaed/room.hpp"
#include "asaed/room_analysis.hpp"
#include "util/log.hpp"
#include "object/tile_map.hpp"
#include "object/tile_set.hpp"
#include "math/random.hpp"

namespace {
	const uint32_t WALL = 2;
	const uint32_t PATH = 1;
	const int WIDTH_BRIDGE = 7;
	const int HEIGHT_BRIDGE = 20;
} // namespace

std::vector<Direction> get_next(const std::unique_ptr<Room>& prev) {
	std::vector<Direction> ret;
	for (int id = 2; id <= 4; ++ id) {
		if (prev && prev->get_next_room() == inverse_direction(static_cast<Direction>(id))) {
			continue;
		}
		ret.push_back(static_cast<Direction>(id));
	}
	assert(!ret.empty());
	return ret;
}

Direction inverse_direction(Direction direction) {
	Direction inverse = Direction::AUTO;
	switch (direction) {
		case Direction::LEFT:
			inverse = Direction::RIGHT;
			break;
		case Direction::RIGHT:
			inverse = Direction::LEFT;
			break;
		case Direction::UP:
			inverse = Direction::DOWN;
			break;
		case Direction::DOWN:
			inverse = Direction::UP;
			break;
		default:
			throw std::runtime_error("Unknown direction of room!!");
			break;
	}
	return inverse;
}

LevelData::LevelData() :
	m_rooms(1),
	m_next_level()
{}

LevelData::~LevelData() {
	m_rooms.clear();
}

void LevelData::add_room(uint32_t id, std::unique_ptr<RoomAnalysis> room) {
	if (id >= static_cast<uint32_t>(m_rooms.size())) {
		m_rooms.resize(id + 1);
	}

	if (m_rooms[id]) {
		log_warning << "RoomAnalysis id " << id << " redefined\n";
	}
	else {
		m_rooms[id] = std::move(room);
	}
}
const RoomAnalysis& LevelData::get(const uint32_t id) const {
	if (id >= static_cast<uint32_t>(m_rooms.size())) {
		log_warning << "RoomAnalysis id " << id << " is invalid\n";
		return *m_rooms[0].get();
	}
	else {
		RoomAnalysis* room = m_rooms[id].get();
		if (room) {
			return *room;
		}
		else {
			log_warning << "RoomAnalysis id " << id << " is invalid\n";
			return *m_rooms[0].get();
		}
	}
}

size_t LevelData::get_num_rooms() const { return m_rooms.size(); }

void LevelData::set_next_level(const std::string& next_level) { m_next_level = next_level; }

Vector get_offset(const std::unique_ptr<Room>& prev, const std::unique_ptr<TileMap>& tilemap) {
	Vector offset;
	if (prev) {
		switch (prev->get_next_room()) {
			case Direction::UP:
				offset = prev->get_bounding_box().get_middle() -
						Vector(tilemap->get_bounding_box().get_width() / 2.0f, 
								(prev->get_bounding_box().get_height() / 2.0f) + tilemap->get_bounding_box().get_height());
				break;
			case Direction::DOWN:
				offset = prev->get_bounding_box().get_middle() +
						Vector(-tilemap->get_bounding_box().get_width() / 2.0f, 
								prev->get_bounding_box().get_height() / 2.0f);
				break;
			case Direction::RIGHT:
				offset = prev->get_bounding_box().get_middle() +
						Vector(prev->get_bounding_box().get_width() / 2.0f,
								-tilemap->get_bounding_box().get_height() / 2.0f);
				break;
			default:
				throw std::runtime_error("Can't make a room with direction left/auto");
				break;
		}
	}
	return offset;
}

std::unique_ptr<Room> LevelData::creat_room(std::unique_ptr<RoomAnalysis> room_analysis, const std::unique_ptr<Room>& prev) const {
	Vector offset = get_offset(prev, room_analysis->m_tilemap);
	room_analysis->m_tilemap->set_offset(offset);
	auto room = std::make_unique<Room>(std::move(room_analysis));
	
	if (prev) {
		room->set_prev_room(inverse_direction(prev->get_next_room()));
	}

	if (room->get_room_type() != RoomType::END) {
		std::vector<Direction> next_direction = get_next(prev);
		int id = g_game_random.rand(0, static_cast<int>(next_direction.size()) - 1);
		room->set_next_room(next_direction[id]);
	}
	
	return room;
}

std::unique_ptr<Room> LevelData::creat_bridge(const std::unique_ptr<Room>& prev) const {
	auto tilemap = std::make_unique<TileMap>(m_tileset);
	switch (prev->get_next_room()) {
		case Direction::UP:
		case Direction::DOWN:
			tilemap->resize(WIDTH_BRIDGE, HEIGHT_BRIDGE);
			for (int i = 0; i < WIDTH_BRIDGE; ++ i) {
				for (int j = 0; j < HEIGHT_BRIDGE; ++ j) {
					if (i == 0 || i == WIDTH_BRIDGE - 1) {
						tilemap->change(i, j, WALL);
					}
					else {
						tilemap->change(i, j, PATH);
					}
				}
			}
			break;
		case Direction::RIGHT:
			tilemap->resize(HEIGHT_BRIDGE, WIDTH_BRIDGE);
			for (int i = 0; i < HEIGHT_BRIDGE; ++ i) {
				for (int j = 0; j < WIDTH_BRIDGE; ++ j) {
					if (j == 0 || j == WIDTH_BRIDGE - 1) {
						tilemap->change(i, j, WALL);
					}
					else {
						tilemap->change(i, j, PATH);
					}
				}
			}
			break;
		default:
			throw std::runtime_error("Can't make a bridge with direction left/auto");
			break;
	}
	
	Vector offset = get_offset(prev, tilemap);
	tilemap->set_offset(offset);
	
	auto room_analysis = std::make_unique<RoomAnalysis>(RoomType::BRIDGE, std::move(tilemap));
	auto room = std::make_unique<Room>(std::move(room_analysis));
	room->set_next_room(prev->get_next_room());
	room->set_prev_room(inverse_direction(prev->get_next_room()));
	return room;
}

std::unique_ptr<Level> LevelData::creat_level_from_leveldata() const {
	auto new_level = std::make_unique<Level>();

	std::unique_ptr<RoomAnalysis> start_room;
	std::unique_ptr<RoomAnalysis> end_room;
	std::unique_ptr<RoomAnalysis> boss_room;
	std::vector<uint32_t> index;
	
	for (size_t id = 1; id < m_rooms.size(); ++ id) {
		const auto& room_analysis = m_rooms[id];
		switch (room_analysis->get_room_type()) {
			case RoomType::START:
				start_room  = room_analysis->clone(Vector(0.0f, 0.0f));
				break;
			case RoomType::END:
				end_room    = room_analysis->clone(Vector(0.0f, 0.0f));
				break;
			case RoomType::BOSS:
				boss_room    = room_analysis->clone(Vector(0.0f, 0.0f));
				break;
			case RoomType::NORMAL:
				index.push_back(id);
				break;
		
		default:
			break;
		}
	}

	// to make random room order
	std::random_shuffle(index.begin(), index.end());
	assert(start_room != nullptr);
	
	new_level->add_room(creat_room(start_room->clone(Vector(0.0f, 0.0f))));
	for (const auto& id : index) {
		new_level->add_room(creat_bridge(new_level->get(new_level->get_num_rooms() - 1)));
		new_level->add_room(creat_room(m_rooms[id]->clone(Vector(0.0f, 0.0f)), new_level->get(new_level->get_num_rooms() - 1)));
	}

	if (boss_room) {
		new_level->add_room(creat_bridge(new_level->get(new_level->get_num_rooms() - 1)));
		new_level->add_room(creat_room(boss_room->clone(Vector(0.0f, 0.0f)), new_level->get(new_level->get_num_rooms() - 1)));
	}

	assert(end_room != nullptr);
	new_level->add_room(creat_bridge(new_level->get(new_level->get_num_rooms() - 1)));
	new_level->add_room(creat_room(end_room->clone(Vector(0.0f, 0.0f)), new_level->get(new_level->get_num_rooms() - 1)));
	new_level->set_next_level(m_next_level);
	return new_level;
}