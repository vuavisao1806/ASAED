#ifndef HEADER_ASAED_WEAPON_WEAPON_HPP
#define HEADER_ASAED_WEAPON_WEAPON_HPP

#include <memory>
#include <stdint.h>
#include <string>

#include "object/moving_sprite.hpp"
#include "math/vector.hpp"

class Weapon final : public MovingSprite {
protected:
	/** m_parent is character to hold a weapon */
	MovingObject* m_parent;

	/** damage of the weapon. will update as soon as possible */
	uint32_t m_hurt_attributes;

	/** position of weapon will draw in character (will be specifically set by m_parent) */
	Vector m_offset;

public:
	Weapon(const Weapon& other);

public:
	~Weapon() override;

public:
	Weapon(const std::string& filename);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

public:
	std::unique_ptr<Weapon> clone() const;

public:
	void set_parent(MovingObject* parent);

	void set_hurt_attributes(uint32_t hurt_attributes);
	uint32_t get_hurt_attributes() const;

	void set_offset(Vector offset);
	Vector get_offset() const;
};

#endif