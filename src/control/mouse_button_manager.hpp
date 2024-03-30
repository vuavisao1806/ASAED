#ifndef HEADER_ASAED_CONTROL_MOUSE_BUTTON_MANAGER_HPP
#define HEADER_ASAED_CONTROL_MOUSE_BUTTON_MANAGER_HPP

#include "control/controller.hpp"
#include "control/mouse_config.hpp"

class InputManager;
struct SDL_MouseButtonEvent;

class MouseButtonManager final {
private:
	InputManager* m_parent;
	MouseConfig& m_mouse_config;

public:
	MouseButtonManager(InputManager* parent, MouseConfig& mouse_config);

private:
	MouseButtonManager(const MouseButtonManager&) = delete;
	MouseButtonManager& operator=(const MouseButtonManager&) = delete;

public:
	void process_mouse_button_event(const SDL_MouseButtonEvent& event);
};

#endif