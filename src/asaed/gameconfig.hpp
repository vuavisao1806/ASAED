#ifndef HEADER_ASAED_ASAED_GAMECONFIG_HPP
#define HEADER_ASAED_ASAED_GAMECONFIG_HPP

#include "control/keyboard_config.hpp"
#include "control/mouse_config.hpp"
#include "math/size.hpp"

/**
 * class to hold some config (but it will be permanent and there will be no change)
*/

class Config final {
public:
	/** 
	 * the width/height to be used to display the game in fullscreen
	 * and it's unique
	 * */
	Size window_size;

	/**
	 * it's funny
	 * it's always 1.0f
	*/
	float magnification;

	/** required when I check collision */
	bool debug;

	KeyboardConfig keyboard_config;
	MouseConfig mouse_config;

public:
	Config();

public:
	void load();
};

#endif