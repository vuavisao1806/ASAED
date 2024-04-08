#ifndef HEADER_ASAED_OBJECT_MOVING_OBJECT_HPP
#define HEADER_ASAED_OBJECT_MOVING_OBJECT_HPP

#include "collision/collision_object.hpp"
#include "collision/collision_system.hpp"
#include "object/game_object.hpp"

/** Class MovingObject is the base for every object that can move in game (used for inheritance) */
class MovingObject : public GameObject, public CollisionObject {
	friend class Room;
	friend class CollisionSystem;
public:
	MovingObject();
	~MovingObject() override;

private:
	MovingObject(const MovingObject&) = delete;
	MovingObject& operator=(const MovingObject&) = delete;

public:
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;

	virtual bool is_valid() const override;

	virtual int get_layer() const = 0;

public:
	static std::string class_name();
	virtual std::string get_class_name() const override;

protected:
	void set_group(CollisionGroup collision_group);
};

#endif