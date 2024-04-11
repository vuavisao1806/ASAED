#ifndef HEADER_ASAED_ASAED_SCREEN_HPP
#define HEADER_ASAED_ASAED_SCREEN_HPP

class Compositor;
class Controller;

/**
 * Class to abstract every types of screen (game_session, ...)
 * Run only active screen (like when pausing the game) with full screen
*/

class Screen {
public:
	virtual ~Screen();

public:
	virtual void draw(Compositor& compositor) = 0;
	virtual void update(float dt_sec, const Controller& controller) = 0;

	/** called before this screen activated to setup some things */
	virtual void setup();
	/** called when the screen paused */
	virtual void leave();
};

#endif