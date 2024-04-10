#include "object/camera.hpp"

#include "asaed/globals.hpp"
#include "asaed/room.hpp"
#include "object/player.hpp"
#include "video/drawing_context.hpp"

Camera::Camera() :
	m_translation(),
	m_screen_size(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT))
{}


Camera::Camera(const Camera& other) :
	m_translation(other.m_translation),
	m_screen_size(other.m_screen_size)
{}

Camera::~Camera()
{}

void Camera::update(float dt_sec) {
	update_normal(dt_sec);
}

void Camera::draw(DrawingContext& drawing_context) {
	m_screen_size = Sizef(drawing_context.get_width(), drawing_context.get_height());
}

std::string Camera::class_name() { return "camera"; }
std::string Camera::get_class_name() const { return class_name(); }

bool Camera::is_singleton() const { return true; }

void Camera::reset(const Vector& pos) {
	m_translation.x = pos.x - m_screen_size.width / 2.0f;
	m_translation.y = pos.y - m_screen_size.height / 2.0f;
}

const Vector Camera::get_translation() const { return m_translation; }
void Camera::set_translation(const Vector& translation) { m_translation = translation; }

void Camera::move(const int dx, const int dy) {
	m_translation.x += static_cast<float>(dx);
	m_translation.y += static_cast<float>(dy);
}

Vector Camera::get_center() const { return m_translation + (m_screen_size.as_vector() / 2.0f); }

const Sizef& Camera::get_screen_size() const { return m_screen_size; }

void Camera::update_normal(float /* dt_sec */) {
	if (!Room::get().get_player().empty()) {
		Player& player = *Room::get().get_player()[0];
		reset(player.get_pos());
	}
}

std::unique_ptr<Camera> Camera::clone() const {
	auto camera = std::make_unique<Camera>(*this);
	return camera;
}