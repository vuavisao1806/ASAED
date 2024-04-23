#include "object/chest.hpp"

#include "asaed/room.hpp"
#include "object/flask_health.hpp"
#include "object/player.hpp"
#include "weapon/weapon.hpp"
#include "weapon/weapon_set.hpp"
#include "math/random.hpp"
#include "util/log.hpp"


Chest::Chest(const Vector& position) :
	MovingSprite(Vector(0.0f, 0.0f), "data/images/object/chest/chest.json"),
	m_weapon(),
	m_spawn(false)
{
	set_pos(position - get_bounding_box().get_middle());
}

void Chest::update(float /* dt_sec */) {}
void Chest::draw(DrawingContext& drawing_context) {
	MovingSprite::draw(drawing_context);

	if (m_weapon) {
		m_weapon->draw(drawing_context);
	}
}

HitResponse Chest::collision(CollisionObject& other, const CollisionHit& /* hit */) {
	if (dynamic_cast<Player*>(&other) && m_spawn == false) {
		set_action("chest-open");
		m_spawn = true;

		int type = g_game_random.rand(0, 2);
		switch (type) {
			case ChessType::FlaskHealthType:
				Room::get().add<FlaskHealth>(get_bounding_box().get_middle());
				break;
			case ChessType::WeaponType: {
				int weapon_id = g_game_random.rand(1, 3);
				m_weapon = std::move(WeaponSet::current()->get(weapon_id).clone(this));
				m_weapon->set_offset(m_weapon->get_bounding_box().get_middle());
				break;
			}
		
			default:
				log_warning << "Chest empty! But it's feature!";
				break;
		}
	}
	return ABORT_MOVE;
}

bool Chest::is_singleton() const { return true; }

void Chest::set_weapon(std::unique_ptr<Weapon> weapon) {
	m_weapon = std::move(weapon); 
	m_weapon->set_offset(m_weapon->get_bounding_box().get_middle());
}

const std::unique_ptr<Weapon>& Chest::get_weapon() { return std::move(m_weapon); }