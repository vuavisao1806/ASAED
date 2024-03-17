#include "video/video_system.hpp"

#include <assert.h>
#include <sstream>

#include "asaed/globals.hpp"
#include "asaed/gameconfig.hpp"
#include "math/size.hpp"
#include "video/painter.hpp"
#include "video/renderer.hpp"
#include "video/texture_manager.hpp"

VideoSystem::VideoSystem() :
	m_sdl_window(nullptr, &SDL_DestroyWindow),
	m_sdl_renderer(nullptr, &SDL_DestroyRenderer),
	m_painter(),
	m_renderer(),
	m_texture_manager()
{
	create_sdl_window();
	create_sdl_renderer();
	apply_config();

	m_painter = std::make_unique<Painter>();
	m_renderer = std::make_unique<Renderer>(m_sdl_renderer.get());
	m_texture_manager = std::make_unique<TextureManager>();
}

VideoSystem::~VideoSystem()
{}

std::unique_ptr<VideoSystem> VideoSystem::create(VideoSystem::VideoType video_system) {
	switch (video_system) {
		case VIDEO_SDL:
			log_info << "new video system (VIDEO_SDL)\n";
			return std::make_unique<VideoSystem>();
			break;
		
		default:
			log_fatal << "invaild video system in config\n";
			assert(false);
			break;
	}
	// It never happened (but Syntax wants this)
	return std::make_unique<VideoSystem>();
}

void VideoSystem::create_sdl_window() {
	Size size = g_config->window_size;
	m_sdl_window.reset(SDL_CreateWindow("ASAED", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.width, size.height, 0));
	
	if(!m_sdl_window) {
		std::ostringstream msg;
		msg << "Couldn't set video " << size.width << 'x' << size.height << ": " << SDL_GetError();
		throw std::runtime_error(msg.str()); 
	}
}

void VideoSystem::create_sdl_renderer() {
	m_sdl_renderer.reset(SDL_CreateRenderer(m_sdl_window.get(), -1, 0));
	if (!m_sdl_renderer) {
		std::ostringstream msg;
		msg << "Couldn't create SDL_Renderer: " << SDL_GetError();
		throw std::runtime_error(msg.str()); 
	}
}

void VideoSystem::apply_config() {
	Rect viewport = Rect(0, 0, g_config->window_size);
	Vector scale = Vector(g_config->magnification, g_config->magnification);
	m_viewport = Viewport(viewport, scale);
}

void VideoSystem::set_icon(const SDL_Surface& icon) {
	SDL_SetWindowIcon(m_sdl_window.get(), const_cast<SDL_Surface*>(&icon));
}

const Viewport& VideoSystem::get_viewport() const { return m_viewport; }
Painter& VideoSystem::get_painter() const { return *m_painter; }
Renderer& VideoSystem::get_renderer() const { return *m_renderer; }

TexturePtr VideoSystem::new_texture(const SDL_Surface& image) {
	return std::make_shared<Texture>(image);
}

void VideoSystem::present() {
	m_renderer->present();
}