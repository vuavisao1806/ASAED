#include "control/mouse_motion_manager.hpp"

#include "control/input_manager.hpp"

MouseMotionManager::MouseMotionManager(InputManager* parent) :
	m_parent(parent)
{}

void MouseMotionManager::process_mouse_motion_event(const SDL_MouseMotionEvent& event) {
	m_parent->get_controller(0).set_cursor_position(Vector(event.x, event.y));
}