#include "asaed/screen_manager.hpp"

#include <assert.h>

#include "asaed/constants.hpp"
#include "asaed/game_session.hpp"
#include "asaed/globals.hpp"
#include "asaed/screen.hpp"
#include "control/input_manager.hpp"
#include "video/compositor.hpp"
#include "util/log.hpp"

ScreenManager::Action::Action(Type type_, std::unique_ptr<Screen> screen_) :
	type(type_),
	screen(std::move(screen_))
{}

ScreenManager::ScreenManager() :
	ms_per_step(static_cast<Uint32>(1000.0f / LOGICAL_FPS)),
	seconds_per_step(static_cast<float>(ms_per_step) / 1000.0f),
	elapsed_ticks(0),
	last_ticks(0),
	m_actions(),
	m_screen_stack()
{}


ScreenManager::~ScreenManager()
{}

void ScreenManager::push_screen(std::unique_ptr<Screen> screen) {
	log_debug << "ScreenManager::push_screen(): " << screen.get() << '\n';
	assert(screen);
	m_actions.push_back(Action(Action::PUSH_ACTION, std::move(screen)));
}

void ScreenManager::pop_screen() {
	log_debug << "ScreenManager::pop_screen(): has stack_size: " << m_screen_stack.size() << '\n';
	m_actions.push_back(Action(Action::POP_ACTION));
}

void ScreenManager::run() {
	handle_screen_switch();
	
	while (!m_screen_stack.empty()) {
		loops();
	}
}

void ScreenManager::quit() {
	m_actions.push_back(Action::QUIT_ACTION);
}

void ScreenManager::handle_screen_switch() {
	while (!m_actions.empty()) {
		// keep track current screen to call Screen::leave()
		auto current_screen = (m_screen_stack.empty() ? nullptr : m_screen_stack.back().get());

		// funny
		auto actions = std::move(m_actions);
		bool quit_everything = false;

		for (auto& action : actions) {
			switch (action.type) {
				case Action::POP_ACTION:
					assert(!m_screen_stack.empty());
					if (current_screen == m_screen_stack.back().get()) {
						m_screen_stack.back()->leave();
						current_screen = nullptr;
					}
					m_screen_stack.pop_back();
					break;

				case Action::PUSH_ACTION:
					assert(action.screen);
					m_screen_stack.push_back(std::move(action.screen));
					break;

				case Action::QUIT_ACTION:
					m_screen_stack.clear();
					current_screen = nullptr;
					quit_everything = true;
					break;

				default:
					break;
			}
		}

		if (!quit_everything) {
			if (current_screen != m_screen_stack.back().get()) {
				if (current_screen != nullptr) {
					current_screen->leave();
				}

				if (!m_screen_stack.empty()) {
					m_screen_stack.back()->setup();
				}
			}
		}
	}
}

void ScreenManager::loops() {
	Uint32 ticks = SDL_GetTicks();
	elapsed_ticks += ticks - last_ticks;
	last_ticks = ticks;
	
	if (elapsed_ticks > ms_per_step * 8) {
		// generaly, when the game loads up or levels are switched
		// the elapsed_ticks grows up extremely large, so we just ignore those large time jumps
		elapsed_ticks = 0;
	}

	if (elapsed_ticks < ms_per_step) {
		SDL_Delay(ms_per_step - elapsed_ticks);
		return;
	}

	g_real_time = static_cast<float>(ticks / 1000.0f);

	int steps = elapsed_ticks / ms_per_step;
	
	// to cap max step per frame
	// 4 steps per frame ~ 16 fps
	// a little bit trolling to handle when lagging
	steps = std::min(steps, 4);

	for (int i = 0; i < steps; ++ i) {
		float dt_sec = seconds_per_step;
		g_game_time += dt_sec;
		process_events();
		update_game_logic(dt_sec);
		elapsed_ticks -= ms_per_step;
	}

	if (steps > 0 && !m_screen_stack.empty()) {
		auto compositor = std::make_unique<Compositor>();
		draw(*compositor.get());
	}

	handle_screen_switch();
}

void ScreenManager::draw(Compositor& compositor) {
	assert(!m_screen_stack.empty());
	m_screen_stack.back()->draw(compositor);

	compositor.render();
}

void ScreenManager::update_game_logic(float dt_sec) {
	// trolling
	Controller& controller = InputManager::current()->get_controller();

	if (!m_screen_stack.empty()) {
		m_screen_stack.back()->update(dt_sec, controller);
	}

	static bool is_active = false; // temporary
	if (controller.hold(Control::ATTACK) && !is_active) {
		is_active = true; // temporary
		pop_screen();
		push_screen(std::make_unique<GameSession>("level1"));
	}
}

void ScreenManager::process_events() {
	InputManager::current()->update();
	
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			quit();
			break;
		}
		InputManager::current()->process_event(event);
	}
}
