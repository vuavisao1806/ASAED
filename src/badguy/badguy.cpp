#include "badguy/badguy.hpp"

#include "asaed/room.hpp"
#include "weapon/hurt.hpp"
#include "object/player.hpp"
#include "object/tile.hpp"
#include "math/random.hpp"
#include "object/floating_text.hpp"
#include "weapon/projectile/projectile.hpp"

BadGuy::BadGuy(const std::string& filename) :
	MovingSprite(Vector(0.0f, 0.0f), filename, LAYER_OBJECT),
	m_health(),
	m_timer_dead(),
	m_radius_wander(),
	m_timer_wander(),
	m_timer_shoot(),
	m_state(STATE_INACTIVE),
	m_direction(Direction::RIGHT),
	m_physic(),
	m_smart_position()
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

HitResponse BadGuy::collision(CollisionObject& other, const CollisionHit& /* hit */) {
	if (auto bullet = dynamic_cast<Projectile*>(&other)) {
		if ((bullet->get_hurt_attributes() & HURT_BADGUY)) {
			m_health -= bullet->get_damage();
			Vector position = Vector(g_game_random.randf(get_bounding_box().get_left(), get_bounding_box().get_right()),
			                         g_game_random.randf(get_bounding_box().get_top(), get_bounding_box().get_bottom()));
			Room::get().add<FloatingText>(position, bullet->get_damage());
			// It shouldn't be placed here but I'm so lazy
			// TODO: update new place
		}
		return ABORT_MOVE;
	}

	if (dynamic_cast<Player*>(&other)) {
		return CONTINUE;
	}
	return ABORT_MOVE;
}

void BadGuy::collision_tile(uint32_t /* tile_attributes */) {

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
	if (Room::get().inside(m_bounding_box) && Room::get().can_see_player(eye)) {
		set_state(STATE_ACTIVE);
	}
	else {
		set_state(STATE_INACTIVE);
	}
}