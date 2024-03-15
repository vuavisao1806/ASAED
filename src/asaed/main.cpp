// #include "asaed/main.hpp"

// #include <SDL.h>
// #include <SDL_image.h>
// #include <iostream>
// #include <string>

// #include "object/player.hpp"
// #include "control/input_manager.hpp"
// #include "control/keyboard_manager.hpp"

// // Screen dimension constants
// const int SCREEN_WIDTH = 1280;
// const int SCREEN_HEIGHT = 800;

// // Starts up SDL and creates window
// bool init();

// // Load media
// bool loadMedia();

// // Frees media and shuts down SDL
// void close();

// // Loads individual image as texture 
// SDL_Texture* loadTexture(std::string path);

// // The window we'll be rendering to
// SDL_Window* gWindow = nullptr;

// // The window renderer
// SDL_Renderer* gRenderer = nullptr;

// // Current displayed texture 
// SDL_Texture* gTexture = nullptr;

// bool init() {
// 	// Initialization flag
// 	bool success = true;

// 	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
// 		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
// 		success = false;
// 	}
// 	else {
// 		// Set teture filtering to linear
// 		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
// 			printf("Warning: Linear texture filtering not enabled!");
// 		}

// 		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
// 		if (gWindow == nullptr) {
// 			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
// 			success = false;
// 		}
// 		else {
// 			// Create renderer for window 
// 			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
// 			if (gRenderer == nullptr) {
// 				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
// 				success = false;
// 			}
// 			else {
// 				// Initialize renderer color
// 				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

// 				// Initialize PNG Loading 
// 				int imgFlags = IMG_INIT_PNG;
// 				if (!IMG_Init(imgFlags) & imgFlags) {
// 					printf("SDL_image could not be created! SDL_image Error: %s\n", IMG_GetError());
// 					success = false;
// 				}	
// 			}
// 		}
// 	}
// 	return success;
// }

// bool loadMedia() {
// 	// Loading success flag
// 	bool success = true;

// 	// Load PNG texture
// 	gTexture = loadTexture( "data/images/m_map.png");
// 	if (gTexture == nullptr) {
// 		printf( "Failed to load texture image!\n" );
// 		success = false;
// 	}

// 	return success;
// }

// void close() {
// 	// Free loaded image
// 	SDL_DestroyTexture( gTexture );
// 	gTexture = nullptr;

// 	// Destroy window	
// 	SDL_DestroyRenderer( gRenderer );
// 	SDL_DestroyWindow( gWindow );
// 	gWindow = nullptr;
// 	gRenderer = nullptr;

// 	// Quit SDL subsystems
// 	IMG_Quit();
// 	SDL_Quit();
// }

// SDL_Texture* loadTexture( std::string path ) {
// 	// The final texture
// 	SDL_Texture* newTexture = nullptr;

// 	// Load image at specified path
// 	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
// 	if (loadedSurface == nullptr) {
// 		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
// 	}
// 	else {
// 		// Create texture from surface pixels
//         newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
// 		if (newTexture == nullptr) {
// 			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
// 		}

// 		// Get rid of old loaded surface
// 		SDL_FreeSurface(loadedSurface);
// 	}
// 	return newTexture;
// }

// int Main::run(int argc, char** argv) {
// 	//Start up SDL and create window
// 	if (!init()) {
// 		printf("Failed to initialize!\n");
// 		return 1;
// 	}
// 	else {
// 		//Load media
// 		if (!loadMedia()) {
// 			printf("Failed to load media!\n");
// 		}
// 		else {	
// 			//Main loop flag
// 			bool quit = false;

// 			//Event handler
// 			SDL_Event e;

// 			Player p(100, 100, Size(16, 28));
// 			p.loadTexture(gRenderer, "data/images/knight/idle-0.png");
			
// 			KeyboardConfig keyboardConfig = KeyboardConfig();
// 			std::unique_ptr<InputManager> control = std::make_unique<InputManager>(keyboardConfig);

// 			//While application is running
// 			while(!quit) {
// 				//Handle events on queue
// 				int dir[2] = {0, 0};
// 				while(SDL_PollEvent(&e) != 0) {
// 					//User requests quit
// 					if (e.type == SDL_QUIT) {
// 						quit = true;
// 					}
					
// 					InputManager::current()->process_event(e);
// 				}

// 				//Clear screen
// 				SDL_RenderClear(gRenderer);

// 				//Render texture to screen
// 				SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

// 				Controller& controller = InputManager::current()->get_controller(0);
// 				if (controller.hold(Control::LEFT) && !controller.hold(Control::RIGHT)) {
// 					dir[0] = -1;
// 				} else if (!controller.hold(Control::LEFT) && controller.hold(Control::RIGHT)) {
// 					dir[0] = 1;
// 				} else {
// 					dir[0] = 0;
// 				}
				
// 				if (controller.hold(Control::UP) && !controller.hold(Control::DOWN)) {
// 					dir[1] = -1;
// 				} else if (!controller.hold(Control::UP) && controller.hold(Control::DOWN)) {
// 					dir[1] = 1;
// 				} else {
// 					dir[1] = 0;
// 				}
				
// 				p.moved(Vector(dir[0] * 4, dir[1] * 4));
// 				p.update();

// 				SDL_Rect temp = Rect(Rectf(p.pos, p.size).to_rect()).to_sdl();
// 				SDL_RenderCopy(gRenderer, p.m_texture, nullptr, &temp);

// 				//Update screen
// 				SDL_RenderPresent(gRenderer);

// 				SDL_Delay(33);
// 			}
// 		}
// 	}

// 	//Free resources and close SDL
// 	close();
// 	return 0;
// }


#include "asaed/main.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>

#include "control/keyboard_config.hpp"
#include "video/renderer.hpp"
#include "util/log.hpp"

#include "object/player.hpp"
#include "video/surface_creator.hpp"
#include "video/texture.hpp"
#include "video/texture_ptr.hpp"

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
	m_input_manager(),
	m_video_system()
{}

Main::~Main() {
	m_input_manager.reset();
	m_video_system.reset();
}

int Main::run(int /* argc */, char** /* argv */) {
	m_sdl_subsystem = std::make_unique<SDLSubsystem>();

	KeyboardConfig keyboardConfig = KeyboardConfig();
	m_input_manager = std::make_unique<InputManager>(keyboardConfig);

	m_video_system = VideoSystem::create(VideoSystem::VIDEO_SDL);

	bool quit = false;
	SDL_Event e;

	std::string map = "data/images/m_map.png";
	SDLSurfacePtr surface = SurfaceCreator::from_file(map);
	TexturePtr texture = std::make_shared<Texture>(*surface);

	Player p(100, 100, Size(16, 28));
	p.loadTexture(VideoSystem::current()->get_renderer().get_sdl_renderer(), "data/images/knight/idle-0.png");
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
		
		SDL_RenderCopy(VideoSystem::current()->get_renderer().get_sdl_renderer(), texture->get_texture(), nullptr, nullptr);
		
		Controller& controller = InputManager::current()->get_controller(0);
		if (controller.hold(Control::LEFT) && !controller.hold(Control::RIGHT)) {
			dir[0] = -1;
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
		

		p.moved(Vector(dir[0] * 6, dir[1] * 6));
		p.update();
		
		SDL_Rect dstrect = Rect(Rectf(p.pos, p.size).to_rect()).to_sdl();
		SDL_RenderCopy(VideoSystem::current()->get_renderer().get_sdl_renderer(), p.m_texture, nullptr, &dstrect);

		VideoSystem::current()->present();
		SDL_Delay(33);
	}
	return 0;
}