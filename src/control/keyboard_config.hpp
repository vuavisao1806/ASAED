#ifndef HEADER_ASAED_CONTROL_KEYBOARD_CONFIG_HPP
#define HEADER_ASAED_CONTROL_KEYBOARD_CONFIG_HPP

#include "control/controller.hpp"

#include <SDL.h>
#include <map>

/**
 * Set up key config
 * And don't do anything else .-.
*/

class KeyboardConfig final {
	friend class KeyboardManager;
public:
	/** I hope I can add two player in the future*/
	class PlayerControl final {
	public:
		int player;
		Control control;

	public:
		bool operator==(const PlayerControl& other) const {
			return (player == other.player && control == other.control);
		}
	};

private:
	std::map<SDL_Keycode, PlayerControl> m_keymap;

public:
	KeyboardConfig();

	/** Use for change key in the future */
	SDL_Keycode reversemap_key(int player, Control c) const;

private:
	KeyboardConfig(const KeyboardConfig&) = delete;
	KeyboardConfig& operator=(const KeyboardConfig&) = delete;
};

#endif