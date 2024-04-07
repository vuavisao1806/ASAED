#include "weapon/weapon.hpp"

#include "badguy/badguy.hpp"
#include "object/player.hpp"
#include "video/canvas.hpp"
#include "util/log.hpp"
#include "weapon/hurt.hpp"

Weapon::Weapon(const std::string& filename) :
	MovingSprite(Vector(0.0f, 0.0f), filename, LAYER_WEAPON),
	m_parent(),
	m_hurt_attributes(),
	m_offset()
{}

Weapon::Weapon(const Weapon& other) :
	MovingSprite(other.get_pos(), other.m_sprite_name, LAYER_WEAPON),
	m_parent(other.m_parent),
	m_hurt_attributes(other.m_hurt_attributes),
	m_offset(other.m_offset)
{}

Weapon::~Weapon() 
{}

void Weapon::update(float /* dt_sec */) {}
void Weapon::draw(DrawingContext& drawing_context) {
	if (m_parent) {
		set_pos(m_parent->get_bounding_box().get_middle() - m_offset);
	}
	else {
		log_warning << "Weapon need character to shoot!!\n";
	}
	MovingSprite::draw(drawing_context);
}

void Weapon::set_parent(MovingObject* parent) {
	if (!parent) {
		log_warning << "The character holding the weapon doesn't exist!!\n";
	}
	m_parent = parent;
	re_set_hurt_attributes();
}

void Weapon::re_set_hurt_attributes() {
	if (m_parent == nullptr) {
		return;
	}
	if (dynamic_cast<Player*>(m_parent)) {
		m_hurt_attributes = HURT_BADGUY;
	}
	if (dynamic_cast<BadGuy*>(m_parent)) {
		m_hurt_attributes = HURT_PLAYER;
	}
}

void Weapon::set_hurt_attributes(uint32_t hurt_attributes) { m_hurt_attributes = hurt_attributes; }
uint32_t Weapon::get_hurt_attributes() const { return m_hurt_attributes; }

void Weapon::set_offset(Vector offset) { m_offset = offset; }
Vector Weapon::get_offset() const { return m_offset; }