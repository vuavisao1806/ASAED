#ifndef HEADER_ASAED_UTIL_TIMER_HPP
#define HEADER_ASAED_UTIL_TIMER_HPP

/** Class to hold time helps update the function of objects */

class Timer final {
private:
	float m_period;
	float m_cycle_start;
	float m_cycle_pause;
	bool m_cyclic;

public:
	Timer();

private:
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

public:
	/** 
	 * start the timer with the given period (in seconds) 
	 * set period to zero to disable timer
	*/
	void start(float period, bool cyclic);

	/** return true if a period (or more) passed */
	bool check();

	/** stop the timer */
	void stop();

	/** pause the timer */
	void pause();

	/** resume (unpause) the timer */
	void resume();

	float get_period() const;
	float get_timeleft() const;
	float get_timegone() const;
	bool started() const;
	bool paused() const;
};

#endif