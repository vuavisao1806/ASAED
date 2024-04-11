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
#include "asaed/level.hpp"
#include "asaed/level_data.hpp"
#include "asaed/game_session.hpp"
#include "asaed/room.hpp"
#include "object/camera.hpp"
#include "object/player.hpp"
#include "object/tile_map.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"

ConfigSubsystem::ConfigSubsystem() {
	g_config = std::make_unique<Config>();
	g_config->load();
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
	m_video_system(),
	m_weapon_set(),
	m_moving_set(),
	m_badguy_manager(),
	m_level_manager(),
	m_screen_manager()
{}

Main::~Main() {
	m_config_subsystem.reset();
	m_sdl_subsystem.reset();
	m_input_manager.reset();
	m_tile_manager.reset();
	m_sprite_manager.reset();
	m_video_system.reset();
	m_weapon_set.reset();
	m_moving_set.reset();
	m_badguy_manager.reset();
	m_level_manager.reset();
	m_screen_manager.reset();
}

int Main::run(int /* argc */, char** /* argv */) {
	m_config_subsystem = std::make_unique<ConfigSubsystem>();
	m_sdl_subsystem = std::make_unique<SDLSubsystem>();

	m_input_manager = std::make_unique<InputManager>(g_config->keyboard_config, g_config->mouse_config);
	
	m_tile_manager = std::make_unique<TileManager>();
	m_sprite_manager = std::make_unique<SpriteManager>();
	m_video_system = VideoSystem::create(VideoSystem::VIDEO_SDL);

	m_weapon_set = std::make_unique<WeaponSet>();
	m_moving_set = std::make_unique<MovingTileSet>();
	m_badguy_manager = std::make_unique<BadGuyManager>();
	m_level_manager = std::make_unique<LevelManager>();
	m_screen_manager = std::make_unique<ScreenManager>();

	m_screen_manager->push_screen(std::make_unique<GameSession>("level1"));
	m_screen_manager->run();
	return 0;
}