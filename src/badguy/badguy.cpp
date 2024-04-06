#include "badguy/badguy.hpp"


BadGuy::BadGuy(const std::string& filename) :
	MovingSprite(Vector(0.0f, 0.0f), filename, LAYER_OBJECT),
	m_timer(),
	m_state(STATE_INACTIVE),
	m_direction(Direction::RIGHT),
	m_physic()
{}

void BadGuy::update(float /* dt_sec */) {}
void BadGuy::draw(DrawingContext& /* drawing_context */) {}

void BadGuy::collision_solid(const CollisionHit& hit) {
	if (hit.left || hit.right) {
		m_physic.set_inverse_velocity_x();
	}
	if (hit.top || hit.bottom) {
		m_physic.set_inverse_velocity_y();
	}
}

HitResponse BadGuy::collision(CollisionObject& other, const CollisionHit& hit) {

}

void BadGuy::collision_tile(uint32_t tile_attributes) {

}

void BadGuy::activated() {}
void BadGuy::deactivated() {}
void BadGuy::active_update(float /* dt_sec */) {}
void BadGuy::inactive_update(float /* dt_sec */) {}
	
void BadGuy::wandering() {}


void BadGuy::set_state(State state) { m_state = state; }
BadGuy::State BadGuy::get_state() const { return m_state; }

void BadGuy::try_active() {

}