
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
	const auto& texture = *request.texture;

	assert(request.srcrects.size() == request.dstrects.size());

	for (size_t i = 0; i < request.dstrects.size(); ++ i) {
		const SDL_Rect& src_rect = to_sdl_rect(request.srcrects[i]);
		const SDL_Rect& dst_rect = to_sdl_rect(request.dstrects[i]);

		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if (request.flip & HORIZONTAL_FLIP) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL);
		if (request.flip & VERTICAL_FLIP) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);
		
		SDL_RenderCopyEx(VideoSystem::current()->get_renderer().get_sdl_renderer(),
		                 texture.get_texture(),
		                 &src_rect, &dst_rect,
		                 0, nullptr, flip);
	}
}