#include "video/surface.hpp"

#include "video/texture_manager.hpp"
#include "video/texture.hpp"
#include "video/video_system.hpp"

Surface::Surface(const TexturePtr& texture, const Flip flip, const std::string& filename = "") :
	m_texture(texture),
	m_region(Rect(0, 0, texture->get_texture_width(), texture->get_image_height())),
	m_flip(flip),
	m_filename(filename)
{}

Surface::Surface(const TexturePtr& texture, const Rect& region, const Flip flip, const std::string& filename = "") :
	m_texture(texture),
	m_region(region),
	m_flip(flip),
	m_filename(filename)
{}

Surface::~Surface()
{}

SurfacePtr Surface::from_texture(const TexturePtr& texture) {
	return std::make_shared<Surface>(texture, NO_FLIP);
}

SurfacePtr Surface::from_file(const std::string& filename, const std::optional<Rect>& rect) {
	if (rect) {
		TexturePtr texture = TextureManager::current()->get(filename, *rect);
		return std::make_shared<Surface>(texture, NO_FLIP, filename);
	}
	else {
		TexturePtr texture = TextureManager::current()->get(filename);
		return std::make_shared<Surface>(texture, NO_FLIP, filename);
	}
}

SurfacePtr Surface::region(const Rect& rect) const {
	return std::make_shared<Surface>(m_texture, rect, m_flip);
}

SurfacePtr Surface::clone_flip(const Flip flip) const {
	return std::make_shared<Surface>(m_texture, m_region, m_flip ^ flip, m_filename);
}

TexturePtr Surface::get_texture() const { return m_texture; }
Rect Surface::get_region() const { return m_region; }
int Surface::get_width() const { return m_region.get_width(); }
int Surface::get_height() const { return m_region.get_height(); }
Flip Surface::get_flip() const { return m_flip; }
std::string Surface::get_filename() const { return m_filename; }