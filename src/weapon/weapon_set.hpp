#ifndef HEADER_ASAED_WEAPON_WEAPON_SET_HPP
#define HEADER_ASAED_WEAPON_WEAPON_SET_HPP

#include "util/currenton.hpp"

#include <memory>
#include <vector>
#include <string>

class ReaderData;
class Weapon;

/** This design like complex between tile_manager, tileset, tileparse */

/**
 * What is WeaponSet?
 * WeaponSet manages every weapon to help clone when needed
 * Weapon is the base class for every weapon
*/

class WeaponSet final : public Currenton<WeaponSet> {
private:
	std::string m_filename;
	std::vector<std::unique_ptr<Weapon>> m_weapons;

public:
	WeaponSet();
	~WeaponSet() override;

private:
	WeaponSet(const WeaponSet&) = delete;
	WeaponSet& operator=(const WeaponSet&) = delete;

public:
	void add_weapon(uint32_t id, std::unique_ptr<Weapon> weapon);
	const Weapon& get(const uint32_t id) const;

public:
	void parse();
	void parse_weapon(const ReaderData* data);
};

#endif