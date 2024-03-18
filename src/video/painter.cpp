
#include "video/painter.hpp"

#include <math.h>
#include <assert.h>

#include "video/video_system.hpp"
#include "video/drawing_request.hpp"
#include "util/log.hpp"
#include "video/texture.hpp"
#include "video/renderer.hpp"

namespace {
	// the reason it appears here is because it's only used in this specific situation
	// the Rect::to_sdl() is 1-1 mapping but it's not working efficiently here
	SDL_Rect to_sdl_rect(const Rectf& rectf) {
		SDL_Rect sdl_rect;

		// floorf is because of int(-0.99f) = int(0.01f) = int(0.99) = 0
		// => a jump in coord at 0
		sdl_rect.x = static_cast<int>(floorf(rectf.get_left()));
		sdl_rect.y = static_cast<int>(floorf(rectf.get_top()));
	
		sdl_rect.w = static_cast<int>(roundf(rectf.get_width()));
		sdl_rect.h = static_cast<int>(roundf(rectf.get_height()));
		
		return sdl_rect;
	}
};

Painter::Painter() :
	m_cliprect()
{}

Painter::~Painter()
{}

void Painter::draw_texture(const TextureRequest& request) {
	const auto& texture = static_cast<const Texture&>(*request.texture);

	assert(request.srcrects.size() == request.dstrects.size());
	assert(request.srcrects.size() == request.angles.size());

	for (size_t i = 0; i < request.dstrects.size(); ++ i) {
		const SDL_Rect& src_rect = to_sdl_rect(request.srcrects[i]);
		const SDL_Rect& dst_rect = to_sdl_rect(request.dstrects[i]);

		uint8_t r = request.color.r8();
		uint8_t g = request.color.g8();
		uint8_t b = request.color.b8();
		uint8_t a = static_cast<uint8_t>(request.alpha * request.color.alpha * 255.0f);

		SDL_SetTextureColorMod(texture.get_texture(), r, g, b);
		SDL_SetTextureAlphaMod(texture.get_texture(), a);

		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if (request.flip & HORIZONTAL_FLIP) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL);
		if (request.flip & VERTICAL_FLIP) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);
		
		SDL_RenderCopyEx(static_cast<Renderer&>(VideoSystem::current()->get_renderer()).get_sdl_renderer(),
		                 texture.get_texture(),
		                 &src_rect, &dst_rect,
		                 request.angles[i], nullptr, flip);
	}
}

void Painter::draw_filled_rect(const FillRectRequest& request) {
	SDL_Rect rect = to_sdl_rect(request.rect);

	uint8_t r = request.color.r8();
	uint8_t g = request.color.g8();
	uint8_t b = request.color.b8();
	uint8_t a = static_cast<uint8_t>(request.alpha * request.color.alpha * 255.0f);

	const auto& m_sdl_renderer = static_cast<Renderer&>(VideoSystem::current()->get_renderer()).get_sdl_renderer();
	SDL_SetRenderDrawBlendMode(m_sdl_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_sdl_renderer, r, g, b, a);
	SDL_RenderFillRect(m_sdl_renderer, &rect);
}

void Painter::set_clip_rect(const Rect& rect) {
	m_cliprect = rect.to_sdl();

	if (SDL_RenderSetClipRect(static_cast<Renderer&>(VideoSystem::current()->get_renderer()).get_sdl_renderer(), &*m_cliprect) < 0) {
		log_warning << "Painter::set_clip_rect() : SDL_RenderSetClipRect failed: " << SDL_GetError() << '\n';
	}
}

void Painter::clear_clip_rect() {
	m_cliprect.reset();

	if (SDL_RenderSetClipRect(static_cast<Renderer&>(VideoSystem::current()->get_renderer()).get_sdl_renderer(), nullptr) < 0) {
		log_warning << "Painter::clear_clip_rect() : SDL_RenderSetClipRect failed: " << SDL_GetError() << '\n';
	}
}