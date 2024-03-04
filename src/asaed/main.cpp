#include "asaed/main.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

// Starts up SDL and creates window
bool init();

// Load media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image as texture 
SDL_Texture* loadTexture(std::string path);

// The window we'll be rendering to
SDL_Window* gWindow = nullptr;

// The window renderer
SDL_Renderer* gRenderer = nullptr;

// Current displayed texture 
SDL_Texture* gTexture = nullptr;

bool init() {
	// Initialization flag
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		// Set teture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == nullptr) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			// Create renderer for window 
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == nullptr) {
				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG Loading 
				int imgFlags = IMG_INIT_PNG;
				if (!IMG_Init(imgFlags) & imgFlags) {
					printf("SDL_image could not be created! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}	
			}
		}
	}
	return success;
}

bool loadMedia() {
	// Loading success flag
	bool success = true;

	// Load PNG texture
	gTexture = loadTexture( "data/images/texture.png");
	if (gTexture == nullptr) {
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

void close() {
	// Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = nullptr;

	// Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path ) {
	// The final texture
	SDL_Texture* newTexture = nullptr;

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		// Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if (newTexture == nullptr) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

int Main::run(int argc, char** argv) {
	//Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
		return 1;
	}
	else {
		//Load media
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while(!quit) {
				//Handle events on queue
				while(SDL_PollEvent(&e) != 0) {
					//User requests quit
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}

				//Clear screen
				SDL_RenderClear(gRenderer);

				//Render texture to screen
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL );

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();
	return 0;
}