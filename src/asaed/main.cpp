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
#include "object/tile.hpp"
#include "object/tile_set.hpp"
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

	// good bye m_surface_screen
	// SurfacePtr m_surface_screen = Surface::from_file("data/images/m_map.png");

	TileSet* m_tile_set = TileManager::current()->get_tileset("data/images/tiles/lever1/lever1-tile.json");

	Player p(100, 100, "data/images/creatures/knight/knight-sprite.json");
	// p.m_sprite->set_action("walk-right");
	const Uint32 ms_per_step = static_cast<Uint32>(1000.0f / LOGICAL_FPS);
	const float seconds_per_step = static_cast<float>(ms_per_step) / 1000.0f;
	Uint32 elapsed_ticks = 0;
	Uint32 last_ticks = 0;
	
	int m_player_go = 0, m_player_last_go = 1;
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

		// drawing_context.push_transform();

		// Rectf dstrect = Rectf(0.0f, 0.0f, m_surface_screen->get_width() / 2.0f, m_surface_screen->get_height() / 2.0f);
		// drawing_context.get_canvas().draw_surface_scaled(m_surface_screen, dstrect, Color::WHITE, LAYER_BACKGROUND);

		// drawing_context.pop_transform();

		drawing_context.push_transform();

		Vector pos(0.0f, 0.0f);
		for (int x = 0; x < static_cast<int>(static_cast<float>(g_config->window_size.width) / BLOCK_SIZE); ++ x) {
			for (int y = 0; y < static_cast<int>(static_cast<float>(g_config->window_size.height) / BLOCK_SIZE); ++ y) {
				m_tile_set->get(1).draw(drawing_context.get_canvas(), pos + Vector(0.0f, BLOCK_SIZE * static_cast<float>(y)), LAYER_BACKGROUND);
			}
			pos += Vector(BLOCK_SIZE, 0.0f);
		}

		drawing_context.pop_transform();

		Controller& controller = InputManager::current()->get_controller(0);
		if (m_player_go != 0) {
			// std::cout << m_player_go << '\n';
			m_player_last_go = m_player_go;
		}
		if (controller.hold(Control::LEFT) && !controller.hold(Control::RIGHT)) {
			dir[0] = -1;
			m_player_go = -1;
		}
		else if (!controller.hold(Control::LEFT) && controller.hold(Control::RIGHT)) {
			dir[0] = 1;
			m_player_go = 1;
		}
		else {
			dir[0] = 0;
			m_player_go = 0;
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
		
		p.moved(Vector(dir[0] * 2, dir[1] * 2));
		p.update();
		
		p.draw(drawing_context.get_canvas(), m_player_go, m_player_last_go);
		// std::cout << m_player_go << ' ' << m_player_last_go << '\n';
		compositor->render();
		// SDL_Delay(33);
	}
	return 0;
}