#include "object/physic.hpp"

Physic::Physic() :
	vx(0.0f),
	vy(0.0f),
	ax(0.0f),
	ay(0.0f)
{}

void Physic::reset() { vx = 0.0f; vy = 0.0f; ax = 0.0f; ay = 0.0f; }

void Physic::set_velocity(float vx_, float vy_) { vx = vx_; vy = vy_; }
void Physic::set_velocity(const Vector& vector) { vx = vector.x; vy = vector.y; }
void Physic::set_velocity_x(float vx_) { vx = vx_; }
void Physic::set_velocity_y(float vy_) { vy = vy_; }

Vector Physic::get_velocity() const { return Vector(vx, vy); }
float Physic::get_velocity_x() const { return vx; }
float Physic::get_velocity_y() const { return vy; }

void Physic::set_acceleration(float ax_, float ay_) { ax = ax_; ay = ay_; }
void Physic::set_acceleration(const Vector& vector) { ax = vector.x; ay = vector.y; }
void Physic::set_acceleration_x(float ax_) { ax = ax_; }
void Physic::set_acceleration_y(float ay_) { ay = ay_; }

Vector Physic::get_acceleration() const { return Vector(ax, ay); }
float Physic::get_acceleration_x() const { return ax; }
float Physic::get_acceleration_y() const { return ay; }

Vector Physic::get_movement(float dt_sec) {
	// this means v * t + 1/2 * a * t^2 (maybe)
	vx += ax * dt_sec;
	vy += ay * dt_sec;
	return Vector(vx, vy) * dt_sec;
}