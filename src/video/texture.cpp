#include "video/texture.hpp"

#include <SDL.h>
#include <sstream>

#include "video/texture_manager.hpp"
#include "video/renderer.hpp"
#include "video/video_system.hpp"

Texture::Texture() :
	m_cache_key(),
	m_texture(nullptr),
	m_width(0),
	m_height(0)
{}

Texture::~Texture() {
	if (TextureManager::current() && m_cache_key) {
		// free cached texture
		TextureManager::current()->release_cache_entry(*m_cache_key);
	}
	SDL_DestroyTexture(m_texture);
}

Texture::Texture(SDL_Texture* texture, int width, int height) :
	m_cache_key(),
	m_texture(texture),
	m_width(width),
	m_height(height)
{}

Texture::Texture(const SDL_Surface& image) :
	m_cache_key(),
	m_width(),
	m_height()
{

	m_texture = SDL_CreateTextureFromSurface(static_cast<Renderer&>(VideoSystem::current()->get_renderer()).get_sdl_renderer(), 
	                                         const_cast<SDL_Surface*>(&image));

	if (!m_texture) {
		std::ostringstream msg;
		msg << "Couldn't create texture: " << SDL_GetError();
		throw std::runtime_error(msg.str());
	}

	m_width = image.w;
	m_height = image.h;
}