#include "weapon/weapon_set.hpp"

#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "util/log.hpp"
#include "weapon/weapon.hpp"
#include "weapon/shooting/shooting.hpp"
#include "weapon/shooting/shooting_analysis.hpp"


WeaponSet::WeaponSet() :
	m_filename("images/weapon/weapon-set.json"),
	m_weapons(1)
{
	parse();
}

WeaponSet::~WeaponSet() {
	m_weapons.clear();
}

void WeaponSet::add_weapon(uint32_t id, std::unique_ptr<Weapon> weapon) {
	if (id >= static_cast<uint32_t>(m_weapons.size())) {
		m_weapons.resize(id + 1);
	}

	if (m_weapons[id]) {
		log_warning << "Weapon id " << id << " redefined\n";
	}
	else {
		m_weapons[id] = std::move(weapon);
	}
}

const Weapon& WeaponSet::get(const uint32_t id) const {
	if (id >= static_cast<uint32_t>(m_weapons.size())) {
		log_warning << "Weapon id " << id << " is invalid\n";
		return *m_weapons[0].get();
	}
	else {
		Weapon* weapon = m_weapons[id].get();
		if (weapon) {
			return *weapon;
		}
		else {
			log_warning << "Weapon id " << id << " is invalid\n";
			return *m_weapons[0].get();
		}
	}
}

void WeaponSet::parse() {
	ReaderMachine reader(m_filename);
	
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse_weapon(reader.get_data(i));
	}
}

void WeaponSet::parse_weapon(const ReaderData* data) {
	uint32_t id;

	if (!data->get("id", id)) {
		throw std::runtime_error("Missing weapon id");
	}

	auto weapon = ShootingAnalysis::from_file(data);
	add_weapon(id, std::move(weapon));
}