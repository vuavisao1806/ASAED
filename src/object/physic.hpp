#ifndef HEADER_ASAED_OBJECT_PHYSIC_HPP
#define HEADER_ASAED_OBJECT_PHYSIC_HPP

#include "math/vector.hpp"

class Physic final {
private:
	/** horizontal and vertical velocity */
	float vx, vy;

	/** horizontal and vertical acceleration */
	float ax, ay;

public:
	Physic();
	
	void reset();

	void set_velocity(float vx_, float vy_);
	void set_velocity(const Vector& vector);
	void set_velocity_x(float vx_);
	void set_velocity_y(float vy_);

	Vector get_velocity() const;
	float get_velocity_x() const;
	float get_velocity_y() const;

	void set_acceleration(float ax_, float ay_);
	void set_acceleration(const Vector& vector);
	void set_acceleration_x(float ax_);
	void set_acceleration_y(float ay_);

	Vector get_acceleration() const;
	float get_acceleration_x() const;
	float get_acceleration_y() const;

	Vector get_movement(float dt_sec);
};

#endif