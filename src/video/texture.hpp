#ifndef HEADER_ASAED_VIDEO_TEXTURE_HPP
#define HEADER_ASAED_VIDEO_TEXTURE_HPP


#include <SDL.h>
#include <string>
#include <tuple>
#include <optional>

#include "math/rect.hpp"

/**
 * This class wraps SDL_Texture* to do things 
 * Key to specific unique character with different texture (convenient to store, find, and get)
*/

class Texture final {
	friend class TextureManager;
public:
	/** filename, left, top, right, bottom */
	using Key = std::tuple<std::string, Rect>;

private:
	std::optional<Key> m_cache_key;

	SDL_Texture* m_texture;
	int m_width;
	int m_height;

public:
	Texture();
	~Texture();
	
public:
	Texture(SDL_Texture* texture, int width, int height);
	Texture(const SDL_Surface& image);

private:
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;


public:
	int get_texture_width() const { return m_width; }
	int get_texture_height() const { return m_height; }

	int get_image_width() const { return m_width; }
	int get_image_height() const { return m_height; }

	SDL_Texture* get_texture() const { return m_texture; }
};

#endif