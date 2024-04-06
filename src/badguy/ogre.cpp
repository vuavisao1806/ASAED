#include "badguy/ogre.hpp"

#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "weapon/weapon.hpp"
#include "weapon/weapon_set.hpp"

namespace {
	const float WALK_SPEED = 100.0f; // That funny setup because I don't think another more beautiful
} // namespace

Ogre::Ogre(const std::string& filename) :
	BadGuy(filename)
{
	m_weapon = WeaponSet::current()->get(1).clone(this);
	m_weapon->set_offset(m_weapon->get_bounding_box().get_middle());
	m_weapon->set_flip(HORIZONTAL_FLIP);
}


void Ogre::update(float dt_sec) {
	switch (m_state) {
		case STATE_INACTIVE:
			inactive_update(dt_sec);
			break;
		case STATE_ACTIVE:
			active_update(dt_sec);
			break;
		
		default:
			throw std::runtime_error("Unknown state bad guy");
			break;
	}
}

void Ogre::draw(DrawingContext& drawing_context) {
	MovingSprite::draw(drawing_context);

	if (m_weapon) {
		m_weapon->draw(drawing_context);
	}
}

void Ogre::activated() {}
void Ogre::deactivated() {}
void Ogre::active_update(float dt_sec) {}
void Ogre::inactive_update(float dt_sec) {}

std::unique_ptr<BadGuy> Ogre::from_file(const ReaderData* data) {
	std::string filename;
	if (!data->get("filename", filename)) {
		throw std::runtime_error("Missing badguy data");
	}
	auto badguy = std::make_unique<Ogre>(data->m_parent_path + filename);
	badguy->m_physic.set_velocity(WALK_SPEED, 0.0f);
	
	float sec_per_turn = 2.0f;
	data->get("sec-per-turn", sec_per_turn);
	badguy->m_timer.start(sec_per_turn, true);

	return badguy;
}

std::unique_ptr<BadGuy> Ogre::clone(const Vector& pos) const {
	auto badguy = std::make_unique<Ogre>(m_sprite_name);
	badguy->set_pos(pos);
	return badguy;
}