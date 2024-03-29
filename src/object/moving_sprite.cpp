#include "object/moving_sprite.hpp"

#include "sprite/sprite_manager.hpp"

MovingSprite::MovingSprite() :
	m_sprite_name(),
	m_sprite(),
	m_layer(),
	m_flip()
{}

MovingSprite::MovingSprite(const Vector& pos, const std::string& sprite_name, 
             int layer, CollisionGroup group) :
	m_sprite_name(sprite_name),
	m_sprite(SpriteManager::current()->create(sprite_name)),
	m_layer(layer),
	m_flip(NO_FLIP)
{
	set_pos(pos);
	set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
	set_group(group);
}

void MovingSprite::update(float /* dt_sec */) {}
void MovingSprite::draw(DrawingContext& drawing_context) {
	m_sprite->draw(drawing_context.get_canvas(), get_pos(), m_layer, m_flip);
}

std::string MovingSprite::class_name() { return "moving-sprite"; }
std::string MovingSprite::get_class_name() const { return class_name(); }

int MovingSprite::get_layer() const { return m_layer; }
void MovingSprite::set_layer(int layer) { m_layer = layer; }

float MovingSprite::get_angle() const { return m_sprite->get_angle(); }
void MovingSprite::set_angle(float angle) { m_sprite->set_angle(angle); }

Flip MovingSprite::get_flip() const { return m_flip; }
void MovingSprite::set_flip(Flip flip) { m_flip = flip; }


void MovingSprite::set_action(const std::string name) {
	m_sprite->set_action(name);
}