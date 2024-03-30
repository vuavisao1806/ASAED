#ifndef HEADER_ASAED_CONTROL_INPUT_MANAGER_HPP
#define HEADER_ASAED_CONTROL_INPUT_MANAGER_HPP

#include "control/controller.hpp"

#include <SDL.h>
#include <vector>
#include <memory>


#include "util/currenton.hpp"

class KeyboardManager;
class KeyboardConfig;

class MouseButtonManager;
class MouseMotionManager;
class MouseConfig;

class InputManager final : public Currenton<InputManager> {
private:
	std::vector<std::unique_ptr<Controller>> m_controllers;

public:
	std::unique_ptr<KeyboardManager> keyboard_manager;
	std::unique_ptr<MouseButtonManager> mouse_button_manager;
	std::unique_ptr<MouseMotionManager> mouse_motion_manager;

public:
	InputManager(KeyboardConfig& keyboard_config, MouseConfig& mouse_config);
	~InputManager() override;

public:
	void update();
	void reset();
	void process_event(const SDL_Event& event);

	Controller& get_controller(int player_id = 0);

	int get_num_users() const;

};

#endif