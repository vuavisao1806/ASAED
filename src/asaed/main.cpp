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

#include "object/player.hpp"
#include "video/drawing_request.hpp"
#include "video/surface_creator.hpp"
#include "video/texture.hpp"
#include "video/texture_ptr.hpp"
#include "video/painter.hpp"

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
	m_video_system()
{}

Main::~Main() {
	m_config_subsystem.reset();
	m_sdl_subsystem.reset();
	m_input_manager.reset();
	m_video_system.reset();
}

int Main::run(int /* argc */, char** /* argv */) {
	m_config_subsystem = std::make_unique<ConfigSubsystem>();
	m_sdl_subsystem = std::make_unique<SDLSubsystem>();

	m_input_manager = std::make_unique<InputManager>(g_config->keyboard_config);

	m_video_system = VideoSystem::create(VideoSystem::VIDEO_SDL);

	bool quit = false;
	SDL_Event e;


	TexturePtr texture = TextureManager::current()->get("data/images/m_map.png");

	Player p(100, 100);
	p.loadTexture("data/images/knight/idle-0.png");
	while (!quit) {
		int dir[2] = {0, 0};
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			InputManager::current()->process_event(e);
		}
		
		VideoSystem::current()->get_renderer().start_draw();
		// assert(VideoSystem::current()->get_renderer().get_sdl_renderer() != nullptr);
		
		std::unique_ptr<TextureRequest> textureRequest = std::make_unique<TextureRequest>();
		textureRequest->texture = texture.get();
		textureRequest->srcrects.push_back(Rectf(0, 0, 1280, 800));
		textureRequest->dstrects.push_back(Rectf(0, 0, 1280, 800));

		VideoSystem::current()->get_painter().draw_texture(*textureRequest);
		textureRequest->srcrects.clear();
		textureRequest->dstrects.clear();

		// SDL_RenderCopy(VideoSystem::current()->get_renderer().get_sdl_renderer(), texture->get_texture(), nullptr, nullptr);
		
		bool m_player_flip = false;
		Controller& controller = InputManager::current()->get_controller(0);
		if (controller.hold(Control::LEFT) && !controller.hold(Control::RIGHT)) {
			dir[0] = -1;
			m_player_flip = true;
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
		

		p.moved(Vector(dir[0] * 16, dir[1] * 16));
		p.update();
		
		Rectf srcrect = Rectf(0, 0, p.m_texture->get_image_width(), p.m_texture->get_image_height());
		Rectf dstrect = Rectf(p.pos, Size(p.m_texture->get_image_width(), p.m_texture->get_image_height()));
		
		textureRequest->texture = p.m_texture.get();
		textureRequest->srcrects.emplace_back(srcrect);
		textureRequest->dstrects.emplace_back(dstrect);

		if (m_player_flip == true) {
			textureRequest->flip = 0b100;
		}

		VideoSystem::current()->get_painter().draw_texture(*textureRequest);
		
		// SDL_RenderCopy(VideoSystem::current()->get_renderer().get_sdl_renderer(), p.m_texture->get_texture(), nullptr, &dstrect);

		VideoSystem::current()->present();
		SDL_Delay(33);
	}
	return 0;
}