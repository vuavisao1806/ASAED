#ifndef HEADER_ASAED_VIDEO_RENDERER_HPP
#define HEADER_ASAED_VIDEO_RENDERER_HPP

#include <SDL.h>

class VideoSystem;

/** This class controls SDL_Renderer* to do things */

class Renderer {
private:
	SDL_Renderer* m_renderer;

public:
	Renderer(SDL_Renderer* renderer);
	~Renderer();

private:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

public:
	
	void start_draw();
	void end_draw();
	
	SDL_Renderer* get_sdl_renderer() const { return m_renderer; }

	void present();
};

#endif