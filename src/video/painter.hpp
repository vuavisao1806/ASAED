#ifndef HEADER_ASAED_VIDEO_PAINTER_HPP
#define HEADER_ASAED_VIDEO_PAINTER_HPP

#include <SDL.h>
#include <optional>

struct DrawingRequest;
struct TextureRequest;
class VideoSystem;

/**
 * Painter is a wrapper to draw things when given a request
 * It was born to avoid hardcode
*/

class Painter final {
	std::optional<SDL_Rect> m_cliprect;

public:
	Painter();
	~Painter();

private:
	Painter(const Painter&) = delete;
	Painter& operator=(const Painter&) = delete;

public:
	void draw_texture(const TextureRequest& request);

};

#endif