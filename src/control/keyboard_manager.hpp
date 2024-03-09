#ifndef HEADER_ASAED_CONTROL_KEYBOARD_MANAGER_HPP
#define HEADER_ASAED_CONTROL_KEYBOARD_MANAGER_HPP

#include "control/controller.hpp"
#include "control/keyboard_config.hpp"

class InputManager;
struct SDL_KeyboardEvent;
struct SDL_TextInputEvent;

class KeyboardManager final {
private:
	InputManager* m_parent;
	KeyboardConfig& m_keyboard_config;

public:
	KeyboardManager(InputManager* parent, KeyboardConfig& keyboard_config);

private:
	KeyboardManager(const KeyboardManager&) = delete;
	KeyboardManager& operator=(const KeyboardManager&) = delete;

public:
	void process_key_event(const SDL_KeyboardEvent& event);
};

#endif