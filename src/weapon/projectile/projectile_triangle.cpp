#include "weapon/projectile/projectile_triangle.hpp"

#include "asaed/room.hpp"
#include "math/vector.hpp"
#include "math/util.hpp"
#include "object/physic.hpp"
#include "weapon/hurt.hpp"
#include "weapon/projectile/projectile_set.hpp"
#include "weapon/projectile/projectile.hpp"
#include "weapon/projectile/projectile_line.hpp"

namespace {
	const float SPEED = 150.0f;
	const float RADIUS = 20.0f;
	const int NUMBER = 4;
}

ProjectileTriangle::ProjectileTriangle(const Vector& position, float angle, int projectile_id) :
	m_fake_position(position),
	m_angle(angle),
	m_projectiles_uid(),
	m_old_angle()
{
	m_physic.set_velocity(math::rotate(Vector(SPEED, 0.0f), angle));
	for (int i = 0; i < 3; ++ i) {
		m_vertical_triangle[i] = position + math::rotate(Vector(0.0f, -RADIUS), m_angle + 120.0f * static_cast<float>(i));
	}

	const auto& projectile = ProjectileSet::current()->get(projectile_id);
	
	auto add_point = [&] (Vector pos) -> void {
		const Rectf rect = Rectf(pos, projectile.get_bounding_box().get_size());
		if (Room::get().inside(rect)) {
			auto& projectile_clone = Room::get().add_object(std::move(projectile.clone(pos, HURT_PLAYER, angle)));
			m_projectiles_uid.push_back(projectile_clone.get_uid());
			m_old_angle.push_back(angle);
		}
		else {
			m_projectiles_uid.push_back({});
			m_old_angle.push_back({});
		}
	};
	
	for (int i = 0; i < 3; ++ i) {
		Vector rhs = m_vertical_triangle[(i + 1) % 3];
		Vector lhs = m_vertical_triangle[i];
		Vector add = (rhs - lhs) / static_cast<float>(NUMBER - 1);
		add_point(lhs);
		
		Vector cur = lhs + add;
		for (int i = 0; i < NUMBER - 2; ++ i, cur += add) {
			add_point(cur);
		}
	}
}

ProjectileTriangle::~ProjectileTriangle() {
	m_projectiles_uid.clear();
	m_old_angle.clear();
}

void ProjectileTriangle::update(float dt_sec) {
	if (all_is_remove()) {
		remove_me();
		return;
	}
	std::vector<Vector> new_position = make_triangle(dt_sec);
	
	for (size_t i = 0; i < m_projectiles_uid.size(); ++ i) {
		if (!m_projectiles_uid[i]) {
			continue;
		}
		auto projectile = Room::get().get_object_by_uid<Projectile>(m_projectiles_uid[i]);
		if (!projectile || !projectile->is_valid()) {
			continue;
		}

		if (!Room::get().inside(projectile->get_bounding_box())) {
			projectile->remove_me();
			continue;
		}
		// sorry physic :(((((
		// projectile->set_velocity((new_position[i] - projectile->get_pos()) / dt_sec);
		// projectile->set_movement(projectile->get_physic().get_movement(dt_sec));
		projectile->set_movement(new_position[i] - projectile->get_pos());
	}
}


void ProjectileTriangle::draw(DrawingContext& /* drawing_context */) {}

std::vector<Vector> ProjectileTriangle::make_triangle(float dt_sec) {
	m_fake_position += m_physic.get_movement(dt_sec);
	m_angle += dt_sec * 360.0f;
	m_angle = std::fmod(m_angle, 360.0f);

	for (int i = 0; i < 3; ++ i) {
		m_vertical_triangle[i] = m_fake_position + math::rotate(Vector(0.0f, -RADIUS), std::fmod(m_angle + 120.0f * static_cast<float>(i), 360.0f));
	}

	
	std::vector<Vector> points;
	for (int i = 0; i < 3; ++ i) {
		Vector rhs = m_vertical_triangle[(i + 1) % 3];
		Vector lhs = m_vertical_triangle[i];
		Vector add = (rhs - lhs) / static_cast<float>(NUMBER - 1);
		points.push_back(lhs);
		
		Vector cur = lhs + add;
		for (int i = 0; i < NUMBER - 2; ++ i, cur += add) {
			points.push_back(cur);
		}
	}
	return points;
}

bool ProjectileTriangle::all_is_remove() const {
	for (size_t i = 0; i < m_projectiles_uid.size(); ++ i) {
		if (!m_projectiles_uid[i]) {
			continue;
		}
		auto projectile = Room::get().get_object_by_uid<Projectile>(m_projectiles_uid[i]);
		if (!projectile || !projectile->is_valid()) {
			continue;
		}
		return false;
	}
	return true;
}