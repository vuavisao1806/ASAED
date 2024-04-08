#ifndef HEADER_ASAED_OBJECT_GAME_OBJECT_MANAGER_HPP
#define HEADER_ASAED_OBJECT_GAME_OBJECT_MANAGER_HPP

#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>

#include "object/game_object.hpp"
#include "util/uid_generator.hpp"

class DrawingContext;
class TileMap;

/**
 * What is GameObjectManager?
 * GameObjectManager holds all of object in game (including map)
 * Room help to handle add, get everything like game_object, map, ...
 * Room is inheritance GameObjectManager to manage object in current room
*/

class GameObjectManager {
private:
	UIDGenerator m_uid_generator;
	std::vector<std::unique_ptr<GameObject>> m_game_objects;
	/** stores objects will be add in next frame (add in flush_game_objects()) */
	std::vector<std::unique_ptr<GameObject>> m_game_objects_new;

	std::unordered_map<UID, GameObject*> m_objects_by_uid;
	std::unordered_map<std::type_index, std::vector<GameObject*>> m_objects_by_type_index;

	/** fast access to solid tilemaps */
	std::vector<TileMap*> m_solid_tilemaps;

public:
	GameObjectManager();
	virtual ~GameObjectManager();

private:
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

public:
	/** add object into queue list to be handle in next frame */
	GameObject& add_object(std::unique_ptr<GameObject> object);
	void clear_object();

	template<typename T, typename... Args>
	T& add(Args&&... args) {
		auto object = std::make_unique<T>(std::forward<Args>(args)...);
		T& object_ref = *object;
		add_object(std::move(object));
		return object_ref;
	}

	template<class T>
	T& get_singleton_by_type() const {
		const auto& range = get_objects_by_type_index(typeid(T));
		assert(range.begin() != range.end());
		assert((*range.begin())->is_singleton());
		return *static_cast<T*>(*range.begin());
	}

	template<class T>
	T* get_object_by_uid(const UID& uid) const {
		auto it = m_objects_by_uid.find(uid);
		if (it == m_objects_by_uid.end()) {
			return nullptr;
		}
		else {
			auto ptr = dynamic_cast<T*>(it->second);
			assert(ptr != nullptr);
			return ptr;
		}
	}

	/** handle adding and deleting objects after one frame (actually) */
	void flush_game_objects();

	void update(float dt_sec);
	void draw(DrawingContext& drawing_context);

	const std::vector<std::unique_ptr<GameObject>>& get_objects() const;
	std::vector<std::unique_ptr<GameObject>>& get_objects_non_const();
	
	const std::vector<TileMap*>& get_solid_tilemaps() const;
	std::vector<TileMap*>& get_solid_tilemaps_non_const();

	const std::vector<GameObject*>& get_objects_by_type_index(std::type_index type_idx) const;

	virtual bool before_object_add(GameObject& object) = 0;
	virtual void before_object_remove(GameObject& object) = 0;

private:
	/** to avoid overriding and do something related object before adding */
	void this_before_object_add(GameObject& object);
	/** to avoid overriding and do something related object before removing */
	void this_before_object_remove(GameObject& object);
};

#endif