#include "asaed/main.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>

#include "asaed/gameconfig.hpp"
#include "asaed/globals.hpp"
#include "control/keyboard_config.hpp"
#include "video/renderer.hpp"
#include "video/texture_manager.hpp"
#include "util/log.hpp"

#include "asaed/constants.hpp"
#include "object/player.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"
#include "video/drawing_request.hpp"
#include "video/layer.hpp"
#include "video/surface_creator.hpp"
#include "video/texture.hpp"
#include "video/texture_ptr.hpp"
#include "video/painter.hpp"
#include "video/surface.hpp"
#include "video/surface_ptr.hpp"

#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"

ConfigSubsystem::ConfigSubsystem() {
	g_config = std::make_unique<Config>();
}

ConfigSubsystem::~ConfigSubsystem() {
	g_config.reset();
}

SDLSubsystem::SDLSubsystem() {
	Uint32 flags = SDL_INIT_VIDEO;
	if (SDL_Init(flags) < 0) {
		std::ostringstream msg;
		msg << "Couldn't init SDL: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags)) {
		std::ostringstream msg;
		msg << "Couldn't init SDL_image: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	atexit(IMG_Quit);
	atexit(SDL_Quit);
}

SDLSubsystem::~SDLSubsystem() {
	IMG_Quit();
	SDL_Quit();
}

Main::Main() :
	m_config_subsystem(),
	m_sdl_subsystem(),
	m_input_manager(),
	m_sprite_manager(),
	m_video_system()
{}

Main::~Main() {
	m_config_subsystem.reset();
	m_sdl_subsystem.reset();
	m_input_manager.reset();
	m_sprite_manager.reset();
	m_video_system.reset();
}

int Main::run(int /* argc */, char** /* argv */) {
	m_config_subsystem = std::make_unique<ConfigSubsystem>();
	m_sdl_subsystem = std::make_unique<SDLSubsystem>();

	m_input_manager = std::make_unique<InputManager>(g_config->keyboard_config);
	
	m_sprite_manager = std::make_unique<SpriteManager>();

	m_video_system = VideoSystem::create(VideoSystem::VIDEO_SDL);


	// ReaderMachine reader("data/images/creatures/knight/knight-sprite.json");
	// std::string name;
	// reader.get_data(0)->get("name", name);
	// std::cout << name << '\n';
	// float fps;
	// reader.get_data(0)->get("fps", fps);
	// std::cout << fps << '\n';
	// std::vector<float> values = {};
	// reader.get_data(0)->get("hitbox", values);
	// for (const auto& val : values) std::cout << val << '\n';
	// std::vector<std::string> images = {};
	// reader.get_data(0)->get("images", images);
	// for (const auto& val : images) std::cout << val << '\n';

	bool quit = false;
	SDL_Event e;


	SurfacePtr m_surface_screen = Surface::from_file("data/images/m_map.png");

	Player p(100, 100, "data/images/creatures/knight/knight-sprite.json");
	// p.m_sprite->set_action("walk-right");
	const Uint32 ms_per_step = static_cast<Uint32>(1000.0f / LOGICAL_FPS);
	const float seconds_per_step = static_cast<float>(ms_per_step) / 1000.0f;
	Uint32 elapsed_ticks = 0;
	Uint32 last_ticks = 0;
	
	while (!quit) {
		Uint32 ticks = SDL_GetTicks();
		elapsed_ticks += ticks - last_ticks;
		last_ticks = ticks;
		
		if (elapsed_ticks > ms_per_step * 8) {
			// for future use
			// generaly, when the game loads up or levels are switched
			// the elapsed_ticks grows up extremely large, so we just ignore those large time jumps
			elapsed_ticks = 0;
		}

		if (elapsed_ticks < ms_per_step) {
			SDL_Delay(ms_per_step - elapsed_ticks);
			continue; // temporary
		}

		g_real_time = static_cast<float>(ticks / 1000.0f);

		int steps = elapsed_ticks / ms_per_step;
		
		// to cap max step per frame
		// 4 steps per frame ~ 16 fps
		// a little bit trolling to handle when lagging
		steps = std::min(steps, 4);

		for (int i = 0; i < steps; ++ i) {
			// for future use
			g_game_time += seconds_per_step;
			
			elapsed_ticks -= ms_per_step;
		}

		int dir[2] = {0, 0};
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			InputManager::current()->process_event(e);
		}

		std::unique_ptr<Compositor> compositor = std::make_unique<Compositor>();
		DrawingContext& drawing_context = compositor->make_context();

		drawing_context.push_transform();

		Rectf dstrect = Rectf(0.0f, 0.0f, m_surface_screen->get_width() / 3.0f, m_surface_screen->get_height() / 3.0f);
		drawing_context.get_canvas().draw_surface_scaled(m_surface_screen, dstrect, Color::WHITE, LAYER_BACKGROUND);

		drawing_context.pop_transform();
		
		bool m_player_go_left = false;
		Controller& controller = InputManager::current()->get_controller(0);
		if (controller.hold(Control::LEFT) && !controller.hold(Control::RIGHT)) {
			dir[0] = -1;
			m_player_go_left = true;
		}
		else if (!controller.hold(Control::LEFT) && controller.hold(Control::RIGHT)) {
			dir[0] = 1;
		}
		else {
			dir[0] = 0;
		}
		
		if (controller.hold(Control::UP) && !controller.hold(Control::DOWN)) {
			dir[1] = -1;
		}
		else if (!controller.hold(Control::UP) && controller.hold(Control::DOWN)) {
			dir[1] = 1;
		}
		else {
			dir[1] = 0;
		}
		
		p.moved(Vector(dir[0] * 3, dir[1] * 3));
		p.update();
		
		p.draw(drawing_context.get_canvas(), m_player_go_left);

		compositor->render();
		// SDL_Delay(33);
	}
	return 0;
}