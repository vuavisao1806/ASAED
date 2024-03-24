#include "object/tile.hpp"

#include "asaed/globals.hpp"
#include "video/canvas.hpp"

Tile::Tile() :
	m_images(),
	m_attributes(0),
	m_fps(8.0f),
	m_name()
{}

Tile::Tile(const std::vector<SurfacePtr>& images, uint32_t attributes, float fps, const std::string& name) :
	m_images(images),
	m_attributes(attributes),
	m_fps(fps),
	m_name(name)
{}


void Tile::draw(Canvas& canvas, const Vector& pos, int layer) const {
	size_t m_frame_index = 0;
	if (m_images.size() > 1) {
		m_frame_index = static_cast<size_t>(g_game_time * m_fps) % m_images.size();
	}
	canvas.draw_surface(m_images[m_frame_index], pos, layer);
}

uint32_t Tile::get_attributes() const { return m_attributes; }
bool Tile::is_solid() const { return (m_attributes & Tile::SOLID); }