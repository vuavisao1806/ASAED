#include "asaed/level.hpp"

#include "asaed/game_session.hpp"
#include "asaed/room_analysis.hpp"
#include "asaed/player_status.hpp"
#include "asaed/save_game.hpp"
#include "asaed/room.hpp"
#include "object/camera.hpp"
#include "object/player.hpp"
#include "object/portal.hpp"
#include "video/drawing_context.hpp"

namespace {
	const uint32_t GATE_DOWN = 3;
	const uint32_t SOLID = 2;
} // namespace

Level::Level() :
	m_rooms(),
	m_next_level()
{}

Level::~Level() {
	m_rooms.clear();
}

void Level::update(float dt_sec) {
	if (!Room::get().get_player().empty()) {
		for (const auto& room : m_rooms) {
			if (Room::current() != room.get()) {
				auto player_ptr = Room::current()->get_player()[0];
				if (room.get()->get_bounding_box().contains(player_ptr->get_bounding_box())) {
					Room::get().move_to(*room.get());
					room->activate();
					room->flush_game_objects();
					break;
				}
			}
		}
	}

	for (const auto& room : m_rooms) {
		room->update(dt_sec);
	}
}

void Level::draw(DrawingContext& drawing_context) {
	for (const auto& room : m_rooms) {
		room->draw(drawing_context);
	}
}

void Level::start_level() {
	for (auto& room : m_rooms) {
		switch (room->get_room_type()) {
			case RoomType::START: {
				auto player = GameSession::current()->get_savegame().get_player_status().clone_player_status();
				player->set_pos(room->get_bounding_box().get_middle());
				room->add_object(std::move(player));
				
				room->add<Camera>();
				room->flush_game_objects();
				room->activate();
				break;
			}
			case RoomType::NORMAL:
				room->update(1.0f);
				break;
			case RoomType::END:
				room->add_object(std::make_unique<Portal>(room->get_bounding_box().get_middle()));
				break;
			default:
				break;
		}
		for (int i = 1; i <= 4; ++ i) {
			Direction m_direction = static_cast<Direction>(i);
			if (m_direction == room->get_prev_room()) continue;
			if (m_direction == room->get_next_room()) continue;
			room->change_gate(m_direction, GATE_DOWN, SOLID);
		}
	}
}

void Level::add_room(std::unique_ptr<Room> room) {
	m_rooms.push_back(std::move(room));
}

void Level::set_next_level(const std::string& next_level) { m_next_level = next_level; }
const std::string& Level::get_next_level() { return m_next_level; }

const std::unique_ptr<Room>& Level::get(uint32_t id) { return m_rooms[id]; }
size_t Level::get_num_rooms() const { return m_rooms.size(); } 
