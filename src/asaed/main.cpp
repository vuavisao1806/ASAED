#include "asaed/main.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <chrono>
#include <random>

#include "asaed/gameconfig.hpp"
#include "asaed/globals.hpp"

#include "math/random.hpp"
#include "asaed/game_session.hpp"
#include "asaed/title_screen.hpp"

ConfigSubsystem::ConfigSubsystem() {
	g_config = std::make_unique<Config>();
	g_config->load();
}

ConfigSubsystem::~ConfigSubsystem() {
	g_config.reset();
}

SDLSubsystem::SDLSubsystem() {
	Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
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

	if(TTF_Init() == -1) {
		std::ostringstream msg;
		msg << "Couldn't init SDL_ttf: " << TTF_GetError();
		throw std::runtime_error(msg.str());
	}

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
		std::ostringstream msg;
		msg << "Couldn't init SDL_mixer: " << Mix_GetError();
		throw std::runtime_error(msg.str());
	}

	atexit(Mix_Quit);
	atexit(TTF_Quit);
	atexit(IMG_Quit);
	atexit(SDL_Quit);
}

SDLSubsystem::~SDLSubsystem() {
	Mix_Quit();
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
	m_screen_manager(),
	m_sound_manager(),
	m_resources(),
	m_game_manager(),
	m_ttf_surface_manager()
{}

Main::~Main() {
	m_config_subsystem.reset();
	m_sdl_subsystem.reset();
	m_input_manager.reset();
	m_resources.reset();
	m_tile_manager.reset();
	m_sprite_manager.reset();
	m_video_system.reset();
	m_weapon_set.reset();
	m_moving_set.reset();
	m_badguy_manager.reset();
	m_level_manager.reset();
	m_screen_manager.reset();
	m_sound_manager.reset();
	m_ttf_surface_manager.reset();
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
	m_sound_manager = std::make_unique<SoundManager>();
	m_resources = std::make_unique<Resources>();
	m_game_manager = std::make_unique<GameManager>();
	m_ttf_surface_manager = std::make_unique<TTFSurfaceManager>();

	// It's like the time I was a student in high school!! So nostalgic
	g_game_random.seed(std::chrono::system_clock::now().time_since_epoch().count());

	SDL_ShowCursor(SDL_DISABLE);

	// m_screen_manager->push_screen(std::make_unique<GameSession>("level1"));
	m_screen_manager->push_screen(std::make_unique<TitleScreen>());
	m_screen_manager->run();
	return 0;
}