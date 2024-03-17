#include "video/renderer.hpp"

#include "video/video_system.hpp"

Renderer::Renderer(SDL_Renderer* renderer) :
	m_renderer(renderer)
{}

Renderer::~Renderer()
{}

void Renderer::start_draw() {
	const Rect& viewport = VideoSystem::current()->get_viewport().get_rect();
	const Vector& scale = VideoSystem::current()->get_viewport().get_scale();

	SDL_Rect sdl_viewport = viewport.to_sdl();

	// SDL_RenderSetViewport() works in scaled screen coordinates (!!)
	// so reset it to (1.0f, 1.0f) to go back standard scale and SDL_RenderSetViewport() love it
	SDL_RenderSetScale(m_renderer, 1.0f, 1.0f);
	SDL_RenderSetViewport(m_renderer, &sdl_viewport);
	SDL_RenderSetScale(m_renderer, scale.x, scale.y);

	SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(m_renderer);
}

void Renderer::end_draw()
{}

void Renderer::present() {
	SDL_RenderPresent(m_renderer);
}