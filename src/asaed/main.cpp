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

#include "video/compositor.hpp"
#include "video/drawing_context.hpp"
#include "object/player.hpp"
#include "video/drawing_request.hpp"
#include "video/surface_creator.hpp"
#include "video/texture.hpp"
#include "video/texture_ptr.hpp"
#include "video/painter.hpp"
#include "video/layer.hpp"

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


	SurfacePtr m_surface_screen = Surface::from_file("data/images/m_map.png");

	Player p(100, 100, "data/images/knight/idle-0.png");
	
	while (!quit) {
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
		
		// Rectf rect = Rectf(100.0f, 100.0f, 200.0f, 200.0f);
		// drawing_context.get_canvas().draw_filled_rect(rect, Color::BLUE, LAYER_OBJECT);

		// drawing_context.pop_transform();

		// compositor->render();

		drawing_context.push_transform();

		Rectf dstrect = Rectf(0.0f, 0.0f, m_surface_screen->get_width() / 2.0f, m_surface_screen->get_height() / 2.0f);
		drawing_context.get_canvas().draw_surface_scaled(m_surface_screen, dstrect, Color::WHITE, LAYER_BACKGROUND);

		drawing_context.pop_transform();
		
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

		drawing_context.push_transform();
		
		if (m_player_flip == false) {
			drawing_context.get_canvas().draw_surface(p.m_surface, p.pos, LAYER_OBJECT);
		}
		else {
			SurfacePtr p_flip = p.m_surface->clone_flip(HORIZONTAL_FLIP);
			drawing_context.get_canvas().draw_surface(p_flip, p.pos, LAYER_OBJECT);
		}

		drawing_context.pop_transform();

		compositor->render();
		SDL_Delay(33);
	}
	return 0;
}