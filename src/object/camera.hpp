#ifndef HEADER_ASAED_OBJECT_CAMERA_HPP
#define HEADER_ASAED_OBJECT_CAMERA_HPP

#include "object/game_object.hpp"

#include <memory>

#include "math/vector.hpp"
#include "math/rectf.hpp"

class Camera final : public GameObject {
private:
	Vector m_translation;
	Sizef m_screen_size;

public:
	Camera();
	Camera(const Camera& other);
	~Camera() override;

private:
	Camera& operator=(const Camera&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	static std::string class_name();
	virtual std::string get_class_name() const override;

	virtual bool is_singleton() const override;

public:
	/** reset camera position */
	void reset(const Vector& pos);

	/** return camera position */
	const Vector get_translation() const;
	void set_translation(const Vector& translation);

	void move(const int dx, const int dy);

	/** return the coord of the point in the center of this camera */
	Vector get_center() const;

	/** return the (width, height) of the screen */
	const Sizef& get_screen_size() const;

	/** to clone camera to other room */
	std::unique_ptr<Camera> clone() const;

private:
	/** follow camera to the player */
	void update_normal(float dt_sec);

};

#endif