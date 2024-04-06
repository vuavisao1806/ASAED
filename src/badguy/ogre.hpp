#ifndef HEADER_ASAED_BADGUY_OGRE_HPP
#define HEADER_ASAED_BADGUY_OGRE_HPP

#include "badguy/badguy.hpp"

class ReaderData;
class Weapon;

class Ogre : public BadGuy {
private:
	std::unique_ptr<Weapon> m_weapon;

public:
	Ogre(const std::string& filename);

private:
	Ogre(const Ogre&) = delete;
	Ogre& operator=(const Ogre&) = delete;

public:
	static std::unique_ptr<BadGuy> from_file(const ReaderData* data);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	virtual void activated();
	virtual void deactivated();
	virtual void active_update(float dt_sec);
	virtual void inactive_update(float dt_sec);

	virtual std::unique_ptr<BadGuy> clone(const Vector& pos) const override;
};

#endif