#include "weapon/shooting/shooting.hpp"

#include "asaed/room.hpp"
#include "weapon/moving_tile/moving_tile.hpp"
#include "weapon/moving_tile/moving_tile_analysis.hpp"
#include "weapon/moving_tile/moving_tile_set.hpp"

Shooting::Shooting(const std::string& filename) :
	Weapon(filename),
	m_attack_per_turn(1),
	m_accuracy(1.0f),
	m_timer()
{}

void Shooting::attack() {
	if (m_timer.check()) {
		for (int i = 0; i < m_attack_per_turn; ++ i) {
			shooting_angle(get_shoot_angle());
		}
	}
}

int Shooting::get_attack_per_turn() const { return m_attack_per_turn; };

void Shooting::shooting_angle(float angle) const {
	const MovingTile& movingtile = MovingTileSet::current()->get(get_moving_tile_id());
	const Rectf rect = Rectf(get_spawn_position(), movingtile.get_bounding_box().get_size());
	if (Room::get().inside(rect)) {
		Room::get().add_object(movingtile.clone(get_spawn_position(), m_hurt_attributes, angle));
	}
}

float Shooting::get_shoot_angle() const { return get_angle(); }

std::string Shooting::class_name() { return "shooting"; }
std::string Shooting::get_class_name() const { return class_name(); };