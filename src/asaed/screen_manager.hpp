#ifndef HEADER_ASAED_ASAED_SCREEN_MANAGER_HPP
#define HEADER_ASAED_ASAED_SCREEN_MANAGER_HPP

#include <SDL.h>

#include <memory>
#include <vector>

#include "util/currenton.hpp"

class Screen;
class Compositor;

class ScreenManager final : public Currenton<ScreenManager> {
private:
	const Uint32 ms_per_step;
	const float seconds_per_step;
	Uint32 elapsed_ticks;
	Uint32 last_ticks;

private:
	struct Action {
		enum Type { PUSH_ACTION, POP_ACTION, QUIT_ACTION };
		Type type;
		std::unique_ptr<Screen> screen;
		Action(Type type_, std::unique_ptr<Screen> screen_ = nullptr);
	};

	std::vector<Action> m_actions;
	std::vector<std::unique_ptr<Screen>> m_screen_stack;

public:
	ScreenManager();
	~ScreenManager() override;

public:
	/** push new screen on screen_stack */
	void push_screen(std::unique_ptr<Screen> screen);
	/** pop back the currently screen out of the screen_stack */
	void pop_screen();

	void run();
	void quit();
	void handle_screen_switch();
	void loops();

private:
	void draw(Compositor& compositor);
	void update_game_logic(float dt_sec);
	void process_events();
};

#endif