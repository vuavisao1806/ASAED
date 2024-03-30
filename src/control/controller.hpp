#ifndef HEADER_ASAED_CONTROL_CONTROLLER_HPP
#define HEADER_ASAED_CONTROL_CONTROLLER_HPP

#include <iosfwd>
#include <string>

#include "math/vector.hpp"

enum class Control {
	LEFT = 0,
	RIGHT,
	UP,
	DOWN,
	
	ATTACK,

	CONTROLCOUNT
};

class Controller {
protected:
	/** current control status */
	bool m_controls[static_cast<int>(Control::CONTROLCOUNT)];

	/** control status at last frame */
	bool m_old_controls[static_cast<int>(Control::CONTROLCOUNT)];

	/**
	 * control position cursor on screen (only one)
	 * if 2 players appear then only one uses the mouse (gun)
	*/
	Vector m_cursor_position;

public:
	Controller();
	virtual ~Controller();

private:
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;

public:
	virtual void update();
	
	void set_control(Control control, bool value);

	/** returns true if the control is pressed down */
	bool hold(Control control) const;

	/** returns true if the control has just been pressed down this frame*/
	bool pressed(Control control) const;

	/** returns true if the control has just been released this frame*/
	bool released(Control control) const;

	void reset();

	template<typename... Control> 
	bool pressed_any(Control&&... controls) const {
		for(const auto& control : {controls... }) {
			if(pressed(control)) {
				return true;
			}
		}
		return false;
	}

	Vector get_cursor_position() const;
	void set_cursor_position(Vector cursor_position);
};

#endif