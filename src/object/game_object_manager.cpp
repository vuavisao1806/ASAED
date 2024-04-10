#include "object/game_object_manager.hpp"

#include <algorithm>

#include "object/tile_map.hpp"
#include "video/drawing_context.hpp"

GameObjectManager::GameObjectManager() :
	m_uid_generator(),
	m_game_objects(),
	m_game_objects_new(),
	m_objects_by_uid(),
	m_objects_by_type_index()
{}

GameObjectManager::~GameObjectManager() {
	assert(m_game_objects.size() == 0);
	assert(m_game_objects_new.size() == 0);
}

GameObject& GameObjectManager::add_object(std::unique_ptr<GameObject> object) {
	assert(object);
	// assert(!object->get_uid());

	if (!object->get_uid()) {
		object->set_uid(m_uid_generator.next());
	}

	GameObject& temp = *object;
	m_game_objects_new.push_back(std::move(object));
	return temp;
}

void GameObjectManager::clear_object() {
	flush_game_objects();

	for (const auto& object : m_game_objects) {
		before_object_remove(*object);
	}

	m_game_objects.clear();
}

void GameObjectManager::flush_game_objects() {
	m_game_objects.erase(
		std::remove_if(
			m_game_objects.begin(), m_game_objects.end(), 
			[this] (const std::unique_ptr<GameObject>& object) {
				if (!object->is_valid()) {
					this_before_object_remove(*object);
					before_object_remove(*object);
					return true;
				}
				else {
					return false;
				}
			}
		),
		m_game_objects.end()
	);

	while (!m_game_objects_new.empty()) {
		auto new_objects = std::move(m_game_objects_new);
		for (auto& object : new_objects) {
			if (before_object_add(*object)) {
				this_before_object_add(*object);
				m_game_objects.push_back(std::move(object));
			}
		}
	}
}

void GameObjectManager::update(float dt_sec) {
	for (const auto& object : m_game_objects) {
		if (object->is_valid()) {
			object->update(dt_sec);
		}
	}
}
void GameObjectManager::draw(DrawingContext& drawing_context) {
	for (const auto& object : m_game_objects) {
		if (object->is_valid()) {
			object->draw(drawing_context);
		}
	}
}

const std::vector<std::unique_ptr<GameObject>>& GameObjectManager::get_objects() const { return m_game_objects; }
std::vector<std::unique_ptr<GameObject>>& GameObjectManager::get_objects_non_const() { return m_game_objects; }

const std::vector<TileMap*>& GameObjectManager::get_solid_tilemaps() const { return m_solid_tilemaps; }
std::vector<TileMap*>& GameObjectManager::get_solid_tilemaps_non_const() { return m_solid_tilemaps; }

const std::vector<GameObject*>& GameObjectManager::get_objects_by_type_index(std::type_index type_idx) const {
	auto it = m_objects_by_type_index.find(type_idx);
	if (it == m_objects_by_type_index.end()) {
		// use a dummy return value to avoid making non-const
		static std::vector<GameObject*> dummy;
		return dummy;
	}
	else {
		return it->second;
	}
}

void GameObjectManager::this_before_object_add(GameObject& object) {
	if (auto tilemap = dynamic_cast<TileMap*>(&object)) {
		if (tilemap->is_solid()) {
			m_solid_tilemaps.push_back(tilemap);
		}
	}

	{
		// by uid
		assert(object.get_uid());
		m_objects_by_uid[object.get_uid()] = &object;
	}

	{
		// by type index
		m_objects_by_type_index[std::type_index(typeid(object))].push_back(&object);
	}
}

void GameObjectManager::this_before_object_remove(GameObject& object) {
	if (auto tilemap = dynamic_cast<TileMap*>(&object)) {
		if (tilemap->is_solid()) {
			m_solid_tilemaps.erase(std::find(m_solid_tilemaps.begin(), m_solid_tilemaps.end(), tilemap));
		}
	}

	{
		// by uid
		m_objects_by_uid.erase(object.get_uid());
	}

	{
		// by type index
		auto& vec = m_objects_by_type_index[std::type_index(typeid(object))];
		auto it = std::find(vec.begin(), vec.end(), &object);
		assert(it != vec.end());
		vec.erase(it);
	}
}