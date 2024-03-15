#include "video/renderer.hpp"

#include "video/video_system.hpp"

Renderer::Renderer(SDL_Renderer* renderer) :
	m_renderer(renderer)
{}

Renderer::~Renderer()
{}

void Renderer::start_draw() {
	SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(m_renderer);
}

void Renderer::end_draw()
{}

void Renderer::present() {
	SDL_RenderPresent(m_renderer);
}