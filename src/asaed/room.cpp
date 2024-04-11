#include "asaed/room.hpp"

#include <limits>
#include <algorithm>

#include "asaed/globals.hpp"
#include "asaed/gameconfig.hpp"
#include "asaed/room_analysis.hpp"
#include "object/camera.hpp"
#include "object/player.hpp"
#include "object/moving_object.hpp"
#include "object/tile_map.hpp"
#include "math/rectf.hpp"
#include "video/drawing_context.hpp"
#include "util/log.hpp"

Room* Room::s_current = nullptr;

namespace {
	const uint32_t GATE_DOWN = 3;
	const uint32_t GATE_UP = 4;
} // namespace

Room::Room() :
	m_room_analysis(),
	m_prev_direction_room(Direction::AUTO),
	m_next_direction_room(Direction::AUTO)
{
	m_collision_system = std::make_unique<CollisionSystem>(*this);
}


Room::Room(std::unique_ptr<RoomAnalysis> room_analysis) :
	m_room_analysis(std::move(room_analysis)),
	m_prev_direction_room(Direction::AUTO),
	m_next_direction_room(Direction::AUTO)
{
	m_room_analysis->set_room_parent(this);
	m_collision_system = std::make_unique<CollisionSystem>(*this);

	add_object(std::move(m_room_analysis->m_tilemap));
	flush_game_objects();
}

Room::~Room() {
	deactivate();
	clear_object();
}

Room& Room::get() {
	assert(s_current != nullptr);
	return *s_current;
}

Room* Room::current() { return s_current; }

void Room::update(float dt_sec) {
	GameObjectManager::update(dt_sec);

	m_room_analysis->update();
	m_collision_system->update();
	
	flush_game_objects();
}

void Room::draw(DrawingContext& drawing_context) {
	drawing_context.push_transform();

	Camera& camera = Room::get().get_camera();
	drawing_context.set_translation(camera.get_translation());
	
	GameObjectManager::draw(drawing_context);
	if (g_config->debug) {
		m_collision_system->draw_debug(drawing_context);
	}

	drawing_context.pop_transform();
}

bool Room::before_object_add(GameObject& object) {
	if (object.is_singleton()) {
		const auto& objects = get_objects_by_type_index(std::type_index(typeid(object)));
		if (!objects.empty()) {
			log_warning << "Can't be inserted one more time GameObject type '" << typeid(object).name() << "'\n";
			return false;
		}
	}
	if (auto moving_object = dynamic_cast<MovingObject*>(&object)) {
		m_collision_system->add(dynamic_cast<CollisionObject*>(moving_object));
	}
	return true;
}

void Room::before_object_remove(GameObject& object) {
	if (auto moving_object = dynamic_cast<MovingObject*>(&object)) {
		m_collision_system->remove(dynamic_cast<CollisionObject*>(moving_object));
	}
}

std::vector<Player*> Room::get_player() const {
	std::vector<Player*> players;
	for (const auto& player_ptr : get_objects_by_type_index(std::type_index(typeid(Player)))) {
		players.push_back(static_cast<Player*>(player_ptr));
	}
	return players;
}

Player* Room::get_nearest_player(const Vector& pos) const {
	Player* nearest_player = nullptr;
	float nearest_distance = std::numeric_limits<float>::max();
	for (const auto& player_ptr : get_objects_by_type_index(std::type_index(typeid(Player)))) {
		Player& player = *static_cast<Player*>(player_ptr);
		float dist = player.get_bounding_box().distance(pos);
		
		if(dist < nearest_distance) {
			nearest_player = &player;
			nearest_distance = dist;
		}
	}
	return nearest_player;
}

bool Room::is_free_of_tiles(const Rectf& rect) const { return m_collision_system->is_free_of_tiles(rect); }
bool Room::free_light_of_sight(const Vector& line_start, const Vector& line_end) const { return m_collision_system->free_light_of_sight(line_start, line_end); }

bool Room::can_see_player(const Vector& eye, Player* player_) const {
	std::vector<Player*> players;
	if (player_) {
		players.push_back(player_);
	}
	else {
		players = get_player();
	}
	for (auto player_ptr : players) {
		Player& player = *static_cast<Player*>(player_ptr);
		// check for free line of sight to all four corners and the middle of the player's bounding box
		if (free_light_of_sight(eye, player.get_bounding_box().p1())) return true;
		if (free_light_of_sight(eye, player.get_bounding_box().p2())) return true;
		if (free_light_of_sight(eye, Vector(player.get_bounding_box().get_right(), player.get_bounding_box().get_top()))) return true;
		if (free_light_of_sight(eye, Vector(player.get_bounding_box().get_left(), player.get_bounding_box().get_bottom()))) return true;
		if (free_light_of_sight(eye, player.get_bounding_box().get_middle())) return true;
	}
	return false;
}

bool Room::inside(const Rectf& rect) const {
	for (const auto& solid : get_solid_tilemaps()) {
		Rectf bounding_box = solid->get_bounding_box();
		if (!bounding_box.contains(rect)) return false;
	}
	return true;
}

Rectf Room::get_bounding_box() const {
	return get_solid_tilemaps()[0]->get_bounding_box();
}

Camera& Room::get_camera() const {
	return get_singleton_by_type<Camera>();
}

void Room::activate() {
	if (s_current != this) {
		s_current->deactivate();
		s_current = this;
	}
	Camera& camera = get_camera();
	camera.update(1.0f);
}

void Room::deactivate() {
	if (s_current != this) {
		return;
	}
	s_current = nullptr;
}

void Room::change_gate(Direction m_direction, uint32_t old_gate, uint32_t new_gate) {
	switch (m_direction) {
		case Direction::LEFT:
			for (const auto& tilemap : get_solid_tilemaps_non_const()) {
				for (int i = 0; i < tilemap->get_height(); ++ i)
				if (tilemap->get_tile_id(0, i) == old_gate) {
					tilemap->change(0, i, new_gate);
				}
			}
			break;
		case Direction::RIGHT:
			for (const auto& tilemap : get_solid_tilemaps_non_const()) {
				for (int i = 0; i < tilemap->get_height(); ++ i)
				if (tilemap->get_tile_id(tilemap->get_width() - 1, i) == old_gate) {
					tilemap->change(tilemap->get_width() - 1, i, new_gate);
				}
			}
			break;
		case Direction::UP:
			for (const auto& tilemap : get_solid_tilemaps_non_const()) {
				for (int i = 0; i < tilemap->get_width(); ++ i)
				if (tilemap->get_tile_id(i, 0) == old_gate) {
					tilemap->change(i, 0, new_gate);
				}
			}
			break;
		case Direction::DOWN:
			for (const auto& tilemap : get_solid_tilemaps_non_const()) {
				for (int i = 0; i < tilemap->get_width(); ++ i)
				if (tilemap->get_tile_id(i, tilemap->get_height() - 1) == old_gate) {
					tilemap->change(i, tilemap->get_height() - 1, new_gate);
				}
			}
			break;
		default:
			break;
	}
}

void Room::move_to(Room& other) {
	for (auto& object : get_objects_non_const()) {
		if (!object->is_valid()) continue;
		if (auto player = dynamic_cast<Player*>(object.get())) {
			other.add_object(player->clone());
			object->remove_me();
		}
		if (auto camera = dynamic_cast<Camera*>(object.get())) {
			other.add_object(camera->clone());
			object->remove_me();
		}
	}
	other.flush_game_objects();
}

void Room::open_room() {
	change_gate(m_prev_direction_room, GATE_UP, GATE_DOWN);
	change_gate(m_next_direction_room, GATE_UP, GATE_DOWN);
}

void Room::start_room() {
	change_gate(m_prev_direction_room, GATE_DOWN, GATE_UP);
	change_gate(m_next_direction_room, GATE_DOWN, GATE_UP);
}

void Room::set_prev_room(Direction direction) { m_prev_direction_room = direction; }
Direction Room::get_prev_room() const { return m_prev_direction_room; }
void Room::set_next_room(Direction direction) { m_next_direction_room = direction; }
Direction Room::get_next_room() const { return m_next_direction_room; }

RoomType Room::get_room_type() const { return m_room_analysis->get_room_type(); }
