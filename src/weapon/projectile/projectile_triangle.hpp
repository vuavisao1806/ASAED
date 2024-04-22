#ifndef HEADER_ASAED_PROJECTILE_PROJECTILE_TRIANGLE_HPP
#define HEADER_ASAED_PROJECTILE_PROJECTILE_TRIANGLE_HPP


#include <memory>
#include <vector>

#include "object/game_object.hpp"
#include "weapon/projectile/projectile_line.hpp"
#include "object/physic.hpp"

class ProjectileTriangle : public GameObject {
private:
	Vector m_fake_position;
	Vector m_vertical_triangle[3];
	Physic m_physic;

	float m_angle;
	std::vector<UID> m_projectiles_uid;
	std::vector<float> m_old_angle;

public:
	ProjectileTriangle(const Vector& position, float angle, int projectile_id);
	~ProjectileTriangle() override;

private:
	ProjectileTriangle(const ProjectileTriangle&) = delete;
	ProjectileTriangle& operator=(const ProjectileTriangle&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	std::vector<Vector> make_triangle(float dt_sec);

	bool all_is_remove() const;
};

#endif