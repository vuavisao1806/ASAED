#ifndef HEADER_ASAED_ASAED_BADGUY_ANUBIS_HPP
#define HEADER_ASAED_ASAED_BADGUY_ANUBIS_HPP

#include "badguy/badguy.hpp"

#include "util/timer.hpp"

class ReaderData;
class Weapon;

class Anubis : public BadGuy {
private:
	int HEALTH;
	Timer m_timer_skill_1;
	
	Timer m_timer_skill_2;

	int m_cnt_shooting_skill_2;
	float m_angle_shooting_skill_2;
	Timer m_timer_recovery_shooting_skill_2;

	Timer m_timer_skill_3;
	Timer m_timer_skill_4;

public:
	Anubis(const std::string& filename);

private:
	Anubis(const Anubis&) = delete;
	Anubis& operator=(const Anubis&) = delete;

public:
	static std::unique_ptr<BadGuy> from_file(const ReaderData* data);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	virtual void wandering() override; 

	virtual void activated() override;
	virtual void deactivated() override;
	virtual void active_update(float dt_sec) override;
	virtual void inactive_update(float dt_sec) override;

	virtual bool is_boss() const override;

	virtual std::unique_ptr<BadGuy> clone(const Vector& pos) const override;

private:
	void skill_1();
	void skill_2();
	void skill_3();
	void skill_4();
};

#endif