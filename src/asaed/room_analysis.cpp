#include "asaed/room_analysis.hpp"

#include "asaed/room.hpp"
#include "asaed/level_data.hpp"
#include "badguy/badguy_manager.hpp"
#include "badguy/badguy.hpp"
#include "math/random.hpp"
#include "object/tile_map.hpp"
#include "object/tile_set.hpp"
#include "util/reader_data.hpp"

namespace {
	const int TRY_TO_SPAWN = 50;
} // namespace

RoomAnalysis::RoomAnalysis() :
	m_parent(nullptr),
	m_room_type(RoomType::NORMAL),
	m_tilemap(),
	m_badguys(),
	m_turns(),
	m_min_turn_spawn_badguy(),
	m_max_turn_spawn_badguy()
{}


RoomAnalysis::RoomAnalysis(RoomType room_type, std::unique_ptr<TileMap> tilemap) :
	m_parent(nullptr),
	m_room_type(room_type),
	m_tilemap(std::move(tilemap)),
	m_badguys(),
	m_turns(),
	m_min_turn_spawn_badguy(),
	m_max_turn_spawn_badguy()
{}

RoomAnalysis::RoomAnalysis(const RoomAnalysis& other) :
	m_parent(other.m_parent),
	m_room_type(other.m_room_type),
	m_tilemap(other.m_tilemap->clone(other.m_tilemap->get_offset())),
	m_badguys(std::move(other.m_badguys)),
	m_turns(other.m_turns),
	m_min_turn_spawn_badguy(other.m_min_turn_spawn_badguy),
	m_max_turn_spawn_badguy(other.m_max_turn_spawn_badguy)
{}

RoomAnalysis::~RoomAnalysis() {
	m_tilemap.reset();
	m_badguys.clear();
}

std::unique_ptr<RoomAnalysis> RoomAnalysis::fromfile(const TileSet* tileset, const ReaderData* data) {
	auto room = std::make_unique<RoomAnalysis>();
	
	std::string type_room;
	if (!data->get("type-room", type_room)) {
		throw std::runtime_error("Missing type room!!");
	}
	std::string filename;
	if (!data->get("filename", filename)) {
		throw std::runtime_error("Missing tilemap in room!!");
	}
	
	data->get("turn", room->m_turns);
	data->get("min-spawn", room->m_min_turn_spawn_badguy);
	data->get("max-spawn", room->m_max_turn_spawn_badguy);
	data->get("badguy", room->m_badguys);

	room->m_room_type = from_string_to_room_type(type_room);
	room->m_tilemap = std::make_unique<TileMap>(tileset, data->m_parent_path + filename);

	return room;
}

void RoomAnalysis::update() {
	switch (m_room_type) {
		case RoomType::START:
		case RoomType::END:
		case RoomType::BRIDGE:
			break;
		case RoomType::NORMAL:
			if (!is_room_clear() && is_turn_clear()) {
				start_next_turn();
			}
			if (!m_parent->get_player().empty()) {
				if (!is_room_clear()) {
					m_parent->start_room();
					// start_next_turn();
				}
			
				if (is_room_clear() && is_turn_clear()) {
					// will update soon
					// add chest
					m_parent->open_room();
				}
			}
	
		default:
			break;
	}
}

std::unique_ptr<RoomAnalysis> RoomAnalysis::clone(const Vector& offset) const {
	auto room = std::make_unique<RoomAnalysis>(*this);
	room->set_offset(offset);
	return room;
}

void RoomAnalysis::set_room_parent(Room* parent) { m_parent = parent; }

void RoomAnalysis::start_next_turn() {
	if (m_turns <= 0) return;
	-- m_turns;

	const auto rect = m_parent->get_bounding_box();
	for (const auto& name_badguy : m_badguys) {
		int number_badguy_per_turn = g_game_random.rand(m_min_turn_spawn_badguy, m_max_turn_spawn_badguy);
		const auto& badguy = BadGuyManager::current()->get(name_badguy);
		Vector position_spawn;
		if (badguy.is_boss()) {
			position_spawn = rect.get_middle();
			if (!m_parent->inside(Rectf(position_spawn, badguy.get_bounding_box().get_size())) ||
			    !m_parent->is_free_of_tiles(Rectf(position_spawn, badguy.get_bounding_box().get_size()))) {
					throw std::runtime_error("Boss needs to be set in the room center!!");
			}
			m_parent->add_object(badguy.clone(position_spawn));
		}
		else {
			for (int i = 1; i <= number_badguy_per_turn; ++ i) {
				// funny!! Because I can't think other solutions smarter unless make file position (but it isn't good and cumbersome)
				for (int test = 1; test <= TRY_TO_SPAWN; ++ test) {
					position_spawn = Vector(g_game_random.randf(rect.get_left(), rect.get_right()),
					                        g_game_random.randf(rect.get_top(), rect.get_bottom()));
					if (!m_parent->inside(Rectf(position_spawn, badguy.get_bounding_box().get_size())) ||
					    !m_parent->is_free_of_tiles(Rectf(position_spawn, badguy.get_bounding_box().get_size()))) {
						continue;
					}
					m_parent->add_object(badguy.clone(position_spawn));
					break;
				}
			}
		}
	}
	m_parent->flush_game_objects();
}

bool RoomAnalysis::is_turn_clear() {
	for (const auto& object : m_parent->get_objects()) {
		if (auto badguy = dynamic_cast<BadGuy*>(object.get())) {
			if (badguy->is_valid()) {
				return false;
			}
		}
	}
	return true;
}

bool RoomAnalysis::is_room_clear() { return !(m_turns > 0 || !is_turn_clear()); }

void RoomAnalysis::set_offset(const Vector& offset) { m_tilemap->set_offset(offset); }

RoomType RoomAnalysis::get_room_type() const { return m_room_type; }