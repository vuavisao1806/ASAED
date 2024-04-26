#ifndef HEADER_ASAED_BADGUY_NECROMANCER_HPP
#define HEADER_ASAED_BADGUY_NECROMANCER_HPP

#include "badguy/badguy.hpp"

#include "util/timer.hpp"

class ReaderData;
class Weapon;

class Necromancer : public BadGuy {
private:
	std::unique_ptr<Weapon> m_weapon;

	Timer m_timer_ultimate;

public:
	Necromancer(const std::string& filename);

private:
	Necromancer(const Necromancer&) = delete;
	Necromancer& operator=(const Necromancer&) = delete;

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
};

#endif