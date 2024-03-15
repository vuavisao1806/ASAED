#include "video/texture_manager.hpp"

#include <SDL_image.h>
#include <assert.h>
#include <sstream>

#include "util/log.hpp"
#include "video/surface_creator.hpp"
#include "video/video_system.hpp"

TextureManager::TextureManager() :
	m_image_textures(),
	m_surfaces()
{}

TextureManager::~TextureManager() {
	for (const auto& texture : m_image_textures) {
		if (!texture.second.expired()) {
			log_warning << "Texture " << std::get<0>(texture.first) << " not freed\n";
		}
	}
	m_image_textures.clear();
	m_surfaces.clear();
}

TexturePtr TextureManager::get(const std::string& filename) {
	Texture::Key key(filename, Rect(0, 0, 0, 0));
	auto it = m_image_textures.find(key);

	TexturePtr texture;
	if (it != m_image_textures.end()) {
		texture = it->second.lock();
	}

	if (!texture) {
		texture = create_image_texture(filename);
		texture->m_cache_key = key;
		m_image_textures[key] = texture;
	}
	return texture;
}

TexturePtr TextureManager::get(const std::string& filename, const std::optional<Rect>& rect) {
	Texture::Key key;
	if (rect) {
		key = Texture::Key(filename, *rect);
	}
	else {
		key = Texture::Key(filename, Rect());
	}
	auto it = m_image_textures.find(key);

	TexturePtr texture;
	if (it != m_image_textures.end()) {
		texture = it->second.lock();
	}

	if (!texture) {
		if (rect) {
			texture = create_image_texture_raw(filename, *rect);
		}
		else {
			texture = create_image_texture(filename);
		}
		texture->m_cache_key = key;
		m_image_textures[key] = texture;
	}
	return texture;
}

const SDL_Surface& TextureManager::get_surface(const std::string& filename) {
	auto it = m_surfaces.find(filename);

	if (it != m_surfaces.end()) {
		return *(it->second);
	}
	else {
		SDLSurfacePtr image = SurfaceCreator::from_file(filename);
		if (!image) {
			std::ostringstream msg;
			msg << "Couldn't load image '" << filename << "' :" << SDL_GetError();
			throw std::runtime_error(msg.str());
		}

		return *(m_surfaces[filename] = std::move(image));
	}
}

void TextureManager::release_cache_entry(const Texture::Key& key) {
	auto it = m_image_textures.find(key);

	if (it == m_image_textures.end()) {
		log_warning << "No cache entry for '" << std::get<0>(key) << "'\n";
	}
	else {
		assert(it->second.expired());
		m_image_textures.erase(it);
	}
}

TexturePtr TextureManager::create_image_texture(const std::string& filename) noexcept {
	try {
		return create_image_texture_raw(filename);
	}
	catch (const std::exception& err) {
		log_warning << "Couldn't load texture '" << filename << "' (now using dummy texture): " << err.what() << '\n';
		return create_dummy_texture();
	}
	// Never occur, add this because syntax likes this
	return TexturePtr();
}

TexturePtr TextureManager::create_image_texture(const std::string& filename, const Rect& rect) noexcept {
	try {
		return create_image_texture_raw(filename, rect);
	}
	catch (const std::exception& err) {
		log_warning << "Couldn't load texture '" << filename << "' (now using dummy texture): " << err.what() << '\n';
		return create_dummy_texture();
	}
	// Never occur, add this because syntax likes this
	return TexturePtr();
}

TexturePtr TextureManager::create_image_texture_raw(const std::string& filename) {
	SDLSurfacePtr image = SurfaceCreator::from_file(filename);
	TexturePtr texture = VideoSystem::current()->new_texture(*image);
	image.reset(nullptr);
	return texture;
}

TexturePtr TextureManager::create_image_texture_raw(const std::string& filename, const Rect& rect) {
	assert(rect.valid());

	const SDL_Surface& surface = get_surface(filename);

	SDLSurfacePtr subimage;
	if (!Rect(0, 0, surface.w, surface.h).contains(rect)) {
		log_warning << filename << ": invalid subregion request: image="
		            << surface.w << "x" << surface.h << ", rect=" << rect << '\n';
	
		subimage = SDLSurfacePtr(SDL_CreateRGBSurface(0, 
		                                              rect.get_width(), 
		                                              rect.get_height(), 
		                                              surface.format->BitsPerPixel,
		                                              surface.format->Rmask,
		                                              surface.format->Gmask,
		                                              surface.format->Bmask,
		                                              surface.format->Amask));

		Rect clipped_rect(std::max(0, rect.left),
		                  std::max(0, rect.top),
		                  std::max(subimage->w, rect.right),
		                  std::max(subimage->h, rect.bottom));
		
		SDL_Rect srcrect = clipped_rect.to_sdl();
		SDL_BlitSurface(const_cast<SDL_Surface*>(&surface), &srcrect, subimage.get(), nullptr);
	}
	else {
		// cast to uint8_t* to make calculation byte wise
		// pitch = numbers byte in 1 width of SDL_Surface
		// BytesPerPixel is the number of bytes used to display the pixel
		// This first argument is the calculation to padding to rect(left, top)
		subimage = SDLSurfacePtr(SDL_CreateRGBSurfaceFrom(static_cast<uint8_t*>(surface.pixels) + 
		                                                  rect.top * surface.pitch +
		                                                  rect.left * surface.format->BytesPerPixel,
		                                                  rect.get_width(), rect.get_height(),
		                                                  surface.format->BitsPerPixel,
		                                                  surface.pitch,
		                                                  surface.format->Rmask,
		                                                  surface.format->Gmask,
		                                                  surface.format->Bmask,
		                                                  surface.format->Amask));
		if (!subimage) {
			throw std::runtime_error("SDL_CreateRGBSurfaceFrom() failed");
		}
	}
	return VideoSystem::current()->new_texture(*subimage);
}

TexturePtr TextureManager::create_dummy_texture() {
	const std::string dummy_texture_filename = "data/images/texture.png";

	// this function will be called when create_image_texture() fail
	// try loading placeholder file
	try {
		TexturePtr dummy_texture = create_image_texture_raw(dummy_texture_filename);
		return dummy_texture;
	}
	catch (const std::exception& err) {
		// if loading placeholder error, try again with using empty surface
		// if also fails, just cry
		SDLSurfacePtr image(SDL_CreateRGBSurface(0, 128, 128, 8, 0, 0, 0, 0));
		if (!image) {
			throw;
		}
		else {
			log_warning << "Couldn't load texture '" << dummy_texture_filename << "' (now using empty one): " << err.what() << '\n';
			TexturePtr texture = VideoSystem::current()->new_texture(*image);
			return texture;
		}
	}
}