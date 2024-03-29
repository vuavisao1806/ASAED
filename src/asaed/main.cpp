#include "asaed/main.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>

#include "asaed/gameconfig.hpp"
#include "asaed/globals.hpp"
#include "util/log.hpp"

#include "asaed/constants.hpp"
#include "object/player.hpp"
#include "object/tile_map.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"

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
	m_tile_manager(),
	m_sprite_manager(),
	m_video_system()
{}

Main::~Main() {
	m_config_subsystem.reset();
	m_sdl_subsystem.reset();
	m_input_manager.reset();
	m_tile_manager.reset();
	m_sprite_manager.reset();
	m_video_system.reset();
}

int Main::run(int /* argc */, char** /* argv */) {
	m_config_subsystem = std::make_unique<ConfigSubsystem>();
	m_sdl_subsystem = std::make_unique<SDLSubsystem>();

	m_input_manager = std::make_unique<InputManager>(g_config->keyboard_config);
	
	m_tile_manager = std::make_unique<TileManager>();

	m_sprite_manager = std::make_unique<SpriteManager>();

	m_video_system = VideoSystem::create(VideoSystem::VIDEO_SDL);


	bool quit = false;
	SDL_Event e;

	TileSet* m_tile_set = TileManager::current()->get_tileset("data/images/lever/lever1/tile/lever1-tile.json");
	TileMap tile_map(m_tile_set, "data/images/lever/lever1/lever1-map.json");

	Player p(0);

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

		std::unique_ptr<Compositor> compositor = std::make_unique<Compositor>();
		DrawingContext& drawing_context = compositor->make_context();
		for (int i = 0; i < steps; ++ i) {
			// now using
			g_game_time += seconds_per_step;

			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				InputManager::current()->process_event(e);
			}


			tile_map.draw(drawing_context);
			// bye bye direct handle player
			p.update(seconds_per_step);
			p.draw(drawing_context);
			
			elapsed_ticks -= ms_per_step;
		}

		if (steps > 0) {
			compositor->render();
		}


	}
	return 0;
}