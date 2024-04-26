#ifndef HEADER_ASAED_ASAED_BADGUY_ZULAN_HPP
#define HEADER_ASAED_ASAED_BADGUY_ZULAN_HPP

#include "badguy/badguy.hpp"

#include "util/timer.hpp"

class ReaderData;
class Weapon;

class Zulan : public BadGuy {
private:
	int HEALTH;
	
	Timer m_timer_skill_1;
	Timer m_timer_skill_2;
	Timer m_timer_skill_3;
	Timer m_timer_skill_3_angry;
	Timer m_timer_skill_4;

public:
	Zulan(const std::string& filename);

private:
	Zulan(const Zulan&) = delete;
	Zulan& operator=(const Zulan&) = delete;

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
	void skill_3_angry();
	void skill_4();
};

#endif