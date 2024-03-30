#include <control/controller.hpp>

Controller::Controller()
{
	reset();
}

Controller::~Controller()
{}

void Controller::update() {
	for (int i = 0; i < static_cast<int>(Control::CONTROLCOUNT); ++ i) {
		m_old_controls[i] = m_controls[i];
	}
}

void Controller::set_control(Control control, bool value) {
	m_controls[static_cast<int>(control)] = value;
}

bool Controller::hold(Control control) const {
	return m_controls[static_cast<int>(control)];
}

bool Controller::pressed(Control control) const {
	return !m_old_controls[static_cast<int>(control)] && m_controls[static_cast<int>(control)];
}

bool Controller::released(Control control) const {
	return m_old_controls[static_cast<int>(control)] && !m_controls[static_cast<int>(control)];
}

void Controller::reset() {
	for (int i = 0; i < static_cast<int>(Control::CONTROLCOUNT); ++ i) {
		m_old_controls[i] = false;
		m_controls[i] = false;
	}
}

Vector Controller::get_cursor_position() const { return m_cursor_position; }

void Controller::set_cursor_position(Vector cursor_position) {
	m_cursor_position = cursor_position;
}