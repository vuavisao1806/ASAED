#include "asaed/room.hpp"

#include <limits>
#include <algorithm>

#include "asaed/globals.hpp"
#include "asaed/gameconfig.hpp"
#include "object/camera.hpp"
#include "object/player.hpp"
#include "object/moving_object.hpp"
#include "object/tile_map.hpp"
#include "math/rectf.hpp"
#include "video/drawing_context.hpp"
#include "util/log.hpp"

Room* Room::s_current = nullptr;

Room::Room() {
	s_current = this;
	m_collision_system = std::make_unique<CollisionSystem>(*this);
}

Room::~Room() { clear_object(); }

Room& Room::get() {
	assert(s_current != nullptr);
	return *s_current;
}

Room* Room::current() { return s_current; }

void Room::update(float dt_sec) {
	GameObjectManager::update(dt_sec);
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