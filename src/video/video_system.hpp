#ifndef HEADER_ASAED_VIDEO_SYSTEM_HPP
#define HEADER_ASAED_VIDEO_SYSTEM_HPP

#include <SDL.h>
#include <memory>

#include "video/texture_ptr.hpp"
#include "video/viewport.hpp"
#include "util/currenton.hpp"
#include "util/log.hpp"

class Painter;
class Renderer;
class TextureManager;

/**
 * What is VideoSystem?
 * General, VideoSystem holds everything to interact (display) in screen
 * VideoSystem holds SDL_Window*, SDL_Renderer* to display
 * VideoSystem holds Renderer, Painter to guarantee consistent and unique
 * VideoSystem holds TextureManager to access the storehouse (Texture and SDLSurfacePtr)
 *
 * Renderer holds SDL_Renderer* (m_sdl_renderer) and config some things before draw
 * Painter is directly drawing everything (actually only drawing requests from canvas)
 * TextureManager is storehouse and monopoly handles process image input
*/

class VideoSystem final : public Currenton<VideoSystem> {
private:
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_sdl_window;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_sdl_renderer;

	Viewport m_viewport;

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
	~VideoSystem() override;

private:
	VideoSystem(const VideoSystem&) = delete;
	VideoSystem& operator=(const VideoSystem&) = delete;

public:
	void set_icon(const SDL_Surface& icon);

	/** hardcoded size of window */
	void create_sdl_window();
	void create_sdl_renderer();
	void apply_config();

public:
	const Viewport& get_viewport() const;
	Painter& get_painter() const;
	Renderer& get_renderer() const;

	TexturePtr new_texture(const SDL_Surface& image);

	void present();
};

#endif