#include "badguy/badguy.hpp"

#include "asaed/room.hpp"
#include "weapon/hurt.hpp"
#include "object/player.hpp"
#include "weapon/moving_tile/moving_tile.hpp"

BadGuy::BadGuy(const std::string& filename) :
	MovingSprite(Vector(0.0f, 0.0f), filename, LAYER_OBJECT),
	m_health(),
	m_timer_dead(),
	m_radius_wander(),
	m_timer_wander(),
	m_timer_shoot(),
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

#include "util/log.hpp"

HitResponse BadGuy::collision(CollisionObject& other, const CollisionHit& hit) {
	if (auto bullet = dynamic_cast<MovingTile*>(&other)) {
		if ((bullet->get_hurt_attributes() & HURT_BADGUY)) {
			m_health -= bullet->get_damage();
			log_info << "badguy health: " << m_health << '\n';
		}
		return ABORT_MOVE;
	}

	if (dynamic_cast<Player*>(&other)) {
		collision_solid(hit);
		return FORCE_MOVE;
	}
	return ABORT_MOVE;
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

void BadGuy::set_start_position(Vector start_position) { m_start_position = start_position; }
Vector BadGuy::get_start_position() const { return m_start_position; }

void BadGuy::try_active() {
	const Vector& eye = get_bounding_box().get_middle();
	if (Room::get().can_see_player(eye)) {
		set_state(STATE_ACTIVE);
	}
	else {
		set_state(STATE_INACTIVE);
	}
}