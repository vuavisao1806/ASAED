#ifndef HEADER_ASAED_ASAED_ROOM_HPP
#define HEADER_ASAED_ASAED_ROOM_HPP

#include <memory>
#include <vector>
#include <stdint.h>

#include "collision/collision_system.hpp"
#include "object/game_object_manager.hpp"
#include "object/direction.hpp"
#include "asaed/room_type.hpp"

class Camera;
class Player;
class Rectf;
class RoomAnalysis;

class Room final : public GameObjectManager {
public:
	friend class CollisionSystem;
	friend class RoomAnalysis;
private:
	static Room* s_current;

	std::unique_ptr<RoomAnalysis> m_room_analysis;
	std::unique_ptr<CollisionSystem> m_collision_system;
	Direction m_prev_direction_room;
	Direction m_next_direction_room;

public:
	Room();
	Room(std::unique_ptr<RoomAnalysis> room_analysis);
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
	Rectf get_bounding_box() const;

	Camera& get_camera() const;

	void activate();
	void deactivate();
	
	void move_to(Room& other);

	void change_gate(Direction m_direction, uint32_t old_gate, uint32_t new_gate);
	void open_room();
	void start_room();

	void set_prev_room(Direction direction);
	Direction get_prev_room() const;
	void set_next_room(Direction direction);
	Direction get_next_room() const;

	RoomType get_room_type() const;
};

#endif