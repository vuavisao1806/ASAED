#ifndef HEADER_ASAED_CONTROL_MOUSE_MOTION_MANAGER_HPP
#define HEADER_ASAED_CONTROL_MOUSE_MOTION_MANAGER_HPP

class InputManager;
struct SDL_MouseMotionEvent;

class MouseMotionManager final {
private:
	InputManager* m_parent;

public:
	MouseMotionManager(InputManager* parent);

private:
	MouseMotionManager(const MouseMotionManager&) = delete;
	MouseMotionManager& operator=(const MouseMotionManager&) = delete;

public:
	void process_mouse_motion_event(const SDL_MouseMotionEvent& event);
};

#endif