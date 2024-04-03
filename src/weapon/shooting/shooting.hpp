#ifndef HEADER_ASAED_WEAPON_SHOOTING_SHOOTING_HPP
#define HEADER_ASAED_WEAPON_SHOOTING_SHOOTING_HPP

#include "weapon/weapon.hpp"
#include "util/timer.hpp"

#include <string>
class Shooting : public Weapon {
protected:
	int m_attack_per_turn;
	float m_accuracy;
	Timer m_timer;

public:
	Shooting(const std::string& filename);

private:
	Shooting(const Shooting&) = delete;
	Shooting& operator=(const Shooting&) = delete;

public:
	virtual void attack() override;
	virtual float get_shoot_angle() const;
	void shooting_angle(float angle) const;
	virtual int get_attack_per_turn() const override;

	virtual uint32_t get_moving_tile_id() const = 0;
	virtual Vector get_spawn_position() const = 0;

	static std::string class_name();
	virtual std::string get_class_name() const override;
};

#endif