#ifndef HEADER_ASAED_CONTROL_MOUSE_CONFIG_HPP
#define HEADER_ASAED_CONTROL_MOUSE_CONFIG_HPP

#include "control/controller.hpp"

#include <SDL.h>
#include <map>


/**
 * Set up mouse config
 * And don't do anything else .-.
*/

/** Actually, I don't have function to change config mouse :(( */

class MouseConfig final {
	friend class MouseButtonManager;
public:
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
	MouseConfig();
	
private:
	MouseConfig(const MouseConfig&) = delete;
	MouseConfig& operator=(const MouseConfig&) = delete;
};


#endif