#ifndef HEADER_ASAED_ASAED_ROOM_HPP
#define HEADER_ASAED_ASAED_ROOM_HPP

#include <memory>
#include <vector>
#include <stdint.h>

#include "collision/collision_system.hpp"
#include "object/game_object_manager.hpp"

class Player;
class Rectf;

class Room final : public GameObjectManager {
public:
	friend class CollisionSystem;

public:
	static Room* s_current;

	std::unique_ptr<CollisionSystem> m_collision_system;

public:
	Room();
	~Room() override;

private:
	Room(const Room&) = delete;
	Room& operator=(const Room&) = delete;

public:
	static Room& get();
	static Room* current();

public:
	void update(float dt_sec);
	void draw(DrawingContext& drawing_context);
	
	virtual bool before_object_add(GameObject& object) override;
	virtual void before_object_remove(GameObject& object) override;

public:
	std::vector<Player*> get_player() const;
	Player* get_nearest_player(const Vector& pos) const;

	bool is_free_of_tiles(const Rectf& rect) const;
	bool free_light_of_sight(const Vector& line_start, const Vector& line_end) const;
	bool can_see_player(const Vector& eye, Player* player_ = nullptr) const;

	bool inside(const Rectf& rect) const;
};

#endif