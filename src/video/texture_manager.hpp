#ifndef HEADER_ASAED_VIDEO_TEXTURE_MANAGER_HPP
#define HEADER_ASAED_VIDEO_TEXTURE_MANAGER_HPP

#include <SDL.h>
#include <memory>
#include <string>
#include <map>

#include "math/rect.hpp"
#include "util/currenton.hpp"
#include "video/surface_creator.hpp"
#include "video/texture.hpp"
#include "video/texture_ptr.hpp"

struct SDL_Surface;

/** 
 * This class stores images and provides way to get them
 * creat image through VideoSystem
*/

class TextureManager final : public Currenton<TextureManager> {
	friend class Texture;
private:
	/** cache textures */
	std::map<Texture::Key, std::weak_ptr<Texture>> m_image_textures;
	/** cache surfaces (not SDL_Surface) */
	std::map<std::string, SDLSurfacePtr> m_surfaces;

public:
	TextureManager();
	~TextureManager() override;

private:
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

public:
	/** to stores textures */
	TexturePtr get(const std::string& filename);
	/** to stores textures */
	TexturePtr get(const std::string& filename, const std::optional<Rect>& rect);

	/** to stores surfaces */
	const SDL_Surface& get_surface(const std::string& filename);

	/** to free cache texture */
	void release_cache_entry(const Texture::Key& key);

	/** on failure a dummy texture is returned and no execption is thrown */
	TexturePtr create_image_texture(const std::string& filename) noexcept;
	/** on failure a dummy texture is returned and no execption is thrown */
	TexturePtr create_image_texture(const std::string& filename, const Rect& rect) noexcept;

	/** throw an exception on error */
	TexturePtr create_image_texture_raw(const std::string& filename);
	/** throw an exception on error */
	TexturePtr create_image_texture_raw(const std::string& filename, const Rect& rect);

	/** use dummy texture in case of failure of create_image_texture() */
	TexturePtr create_dummy_texture();
};


#endif