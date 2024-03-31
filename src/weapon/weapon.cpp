#include "weapon/weapon.hpp"

#include "video/canvas.hpp"
#include "util/log.hpp"

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
		// log_warning << m_parent->get_pos() << ' ' << m_offset << '\n';
		set_pos(m_parent->get_bounding_box().get_middle() - m_offset);
	}
	else {
		log_warning << "Weapon need character to shoot!!\n";
	}
	MovingSprite::draw(drawing_context);
}

std::unique_ptr<Weapon> Weapon::clone() const {
	return std::make_unique<Weapon>(*this);
}

void Weapon::set_parent(MovingObject* parent) {
	if (!parent) {
		log_warning << "The character holding the weapon doesn't exist!!\n";
	}
	m_parent = parent;
}

void Weapon::set_hurt_attributes(uint32_t hurt_attributes) { m_hurt_attributes = hurt_attributes; }
uint32_t Weapon::get_hurt_attributes() const { return m_hurt_attributes; }

void Weapon::set_offset(Vector offset) { m_offset = offset; }
Vector Weapon::get_offset() const { return m_offset; }