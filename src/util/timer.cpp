#include "util/timer.hpp"

#include <cmath>

#include "asaed/globals.hpp"

Timer::Timer() :
	m_period(0),
	m_cycle_start(),
	m_cycle_pause(),
	m_cyclic(false)
{}

void Timer::start(float period, bool cyclic) {
	m_period = period;
	m_cycle_start = g_game_time;
	m_cycle_pause = 0;
	m_cyclic = cyclic;
}

bool Timer::check() {
	if (m_period == 0.0f) return false;
	
	if (g_game_time - m_cycle_start >= m_period) {
		if (m_cyclic) {
			m_cycle_start = g_game_time - std::fmod(g_game_time - m_cycle_start, m_period);
		}
		else {
			m_period = 0;
		}
		return true;
	}
	return false;
}

void Timer::stop() {
	start(0, false);
}

void Timer::pause() {
	float left = get_timeleft();
	stop();
	m_cycle_pause = left;
}

void Timer::resume() { start(m_cycle_pause, true); }

float Timer::get_period() const { return m_period; }
float Timer::get_timeleft() const { return m_period - (g_game_time - m_cycle_start); }
float Timer::get_timegone() const { return g_game_time - m_cycle_start; }
bool Timer::started() const { return (m_period != 0.0f && get_timeleft() > 0); }
bool Timer::paused() const { return m_cycle_pause != 0; }