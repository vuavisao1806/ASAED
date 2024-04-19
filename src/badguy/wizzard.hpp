#ifndef HEADER_ASAED_BADGUY_WIZZARD_HPP
#define HEADER_ASAED_BADGUY_WIZZARD_HPP

#include "badguy/badguy.hpp"

class ReaderData;
class Weapon;

class Wizzard : public BadGuy {
private:
	std::unique_ptr<Weapon> m_weapon;

public:
	Wizzard(const std::string& filename);

private:
	Wizzard(const Wizzard&) = delete;
	Wizzard& operator=(const Wizzard&) = delete;

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