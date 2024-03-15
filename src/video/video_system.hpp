#ifndef HEADER_ASAED_VIDEO_SYSTEM_HPP
#define HEADER_ASAED_VIDEO_SYSTEM_HPP

#include <SDL.h>
#include <memory>

#include "video/texture_ptr.hpp"
#include "util/currenton.hpp"
#include "util/log.hpp"

class Painter;
class Renderer;
class TextureManager;

class VideoSystem final : public Currenton<VideoSystem> {
private:
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_sdl_window;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_sdl_renderer;

	std::unique_ptr<Painter> m_painter;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<TextureManager> m_texture_manager;

public:
	enum VideoType {
		VIDEO_SDL
	};
	
	static std::unique_ptr<VideoSystem> create(VideoSystem::VideoType video_system);

public:
	VideoSystem();
	~VideoSystem();

private:
	VideoSystem(const VideoSystem&) = delete;
	VideoSystem& operator=(const VideoSystem&) = delete;

public:
	void set_icon(const SDL_Surface& icon);

	/** hardcoded size of window */
	void create_sdl_window();
	void create_sdl_renderer();

public:
	Painter& get_painter() const;
	Renderer& get_renderer() const;

	TexturePtr new_texture(const SDL_Surface& image);

	void present();
};

#endif