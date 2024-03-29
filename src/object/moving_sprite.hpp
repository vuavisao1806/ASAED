#ifndef HEADER_ASAED_OBJECT_MOVING_SPRITE_HPP
#define HEADER_ASAED_OBJECT_MOVING_SPRITE_HPP

#include "object/moving_object.hpp"
#include "sprite/sprite.hpp"
#include "sprite/sprite_ptr.hpp"
#include "video/drawing_context.hpp"
#include "video/layer.hpp"

class MovingSprite : public MovingObject {
protected:
	std::string m_sprite_name;

	SpritePtr m_sprite;
	int m_layer;
	Flip m_flip;

public:
	MovingSprite();
	MovingSprite(const Vector& pos, const std::string& sprite_name, 
	             int layer = LAYER_OBJECT, CollisionGroup group = COLLISION_MOVING);

private:
	MovingSprite(const MovingSprite&) = delete;
	MovingSprite& operator=(const MovingSprite&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	static std::string class_name();
	virtual std::string get_class_name() const override;

	virtual int get_layer() const override;
	void set_layer(int layer);

	float get_angle() const;
	void set_angle(float angle);

	Flip get_flip() const;
	void set_flip(Flip flip);

public:
	void set_action(const std::string name);
};

#endif