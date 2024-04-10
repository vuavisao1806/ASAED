#include "object/tile_map.hpp"

#include <assert.h>
#include <cmath>

#include "asaed/constants.hpp"
#include "object/tile.hpp"
#include "object/tile_set.hpp"
#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"
#include "video/drawing_context.hpp"
#include "video/layer.hpp"

TileMap::TileMap(const TileSet* tileset) :
	m_tileset(tileset),
	m_tiles(),
	m_is_solid(true),
	m_width(),
	m_height(),
	m_layer(LAYER_BACKGROUND),
	m_offset()
{}


TileMap::TileMap(const TileMap& other) :
	m_tileset(other.m_tileset),
	m_tiles(other.m_tiles),
	m_is_solid(true),
	m_width(other.m_width),
	m_height(other.m_height),
	m_layer(other.m_layer),
	m_offset(other.m_offset)
{}

TileMap::TileMap(const TileSet* tileset, const std::string& filename) :
	TileMap(tileset)
{
	parse(filename);
}

TileMap::~TileMap() {
	m_tiles.clear();
}

// std::unique_ptr<TileMap> TileMap::fromfile(const TileSet* tileset, const std::string& filename) {
// 	std::unique_ptr<TileMap> tilemap = std::make_unique<TileMap>(tileset);
// }

void TileMap::update(float /* dt_sec */) {}

#include "util/log.hpp"

void TileMap::draw(DrawingContext& context) {
	context.push_transform();
	
	Canvas& canvas = context.get_canvas();

	Rectf draw_rect = context.get_cliprect();
	// log_warning << "current: " << draw_rect << '\n';
	// log_warning << "bouding box: " << get_bounding_box() << '\n';
	Rect t_draw_rect = get_tiles_overlapping(draw_rect);
	// log_warning << "fake: " << t_draw_rect << '\n';
	// log_warning << "offset: " << m_offset << '\n';
	Vector start_draw = get_tile_position(t_draw_rect.get_left(), t_draw_rect.get_top());
	// log_warning << "real: " << start_draw << '\n';
	Vector pos(0.0f, 0.0f);
	int tx = 0;
	int ty = 0;

	for (pos.x = start_draw.x, tx = t_draw_rect.get_left(); tx < t_draw_rect.get_right(); pos.x += BLOCK_SIZE, ++ tx) {
		for (pos.y = start_draw.y, ty = t_draw_rect.get_top(); ty < t_draw_rect.get_bottom(); pos.y += BLOCK_SIZE, ++ ty) {
			int index = ty * m_width + tx;
			assert(index >= 0);
			assert(index < m_width * m_height);
			// log_warning << "pos: " << pos << '\n';
			if (m_tiles[index] == 0) continue;
			const Tile& tile = m_tileset->get(m_tiles[index]);
			tile.draw(canvas, pos, m_layer);
		}
	}

	// log_warning << "------------------------\n";
	context.pop_transform();
}

int TileMap::get_width() const { return m_width; }
int TileMap::get_height() const { return m_height; }
Size TileMap::get_size() const { return Size(m_width, m_height); }

bool TileMap::is_solid() const { return m_is_solid; }

void TileMap::set_offset(const Vector& offset) { m_offset = offset; }
Vector TileMap::get_offset() const { return m_offset; }

void TileMap::resize(int newWidth, int newHeight) {
	if (newWidth <= 0 || newHeight <= 0) {
		throw std::runtime_error("width/height unknown");
	}
	m_width= newWidth;
	m_height = newHeight;
	m_tiles.resize(m_width * m_height);
}

uint32_t TileMap::get_tile_id(int x, int y) const {
	if (x < 0) x = 0;
	if (x >= m_width) x = m_width - 1;
	if (y < 0) y = 0;
	if (y >= m_height) y = m_height - 1;
	return m_tiles[y * m_width + x];
}

uint32_t TileMap::get_tile_id_at(const Vector& pos) const {
	Vector m_pos = (pos - m_offset) / BLOCK_SIZE;
	return get_tile_id(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y));
}

const Tile& TileMap::get_tile(int x, int y) const {
	return m_tileset->get(get_tile_id(x, y));
}

const Tile& TileMap::get_tile_at(const Vector& pos) const {
	return m_tileset->get(get_tile_id_at(pos));
}

void TileMap::change(int x, int y, uint32_t new_tile) {
	assert(0 <= x && x < m_width && 0 <= y && y < m_height);
	m_tiles[y * m_width + x] = new_tile;
}

void TileMap::change_at(const Vector& pos, uint32_t new_tile) {
	Vector m_pos = (pos - m_offset) / BLOCK_SIZE;
	change(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), new_tile);
}

Vector TileMap::get_tile_position(int x, int y) const {
	return m_offset + Vector(static_cast<float>(x), static_cast<float>(y)) * BLOCK_SIZE;
}

Rectf TileMap::get_bounding_box() const {
	return Rectf(get_tile_position(0, 0), get_tile_position(m_width, m_height));
}

Rectf TileMap::get_tile_bounding_box(int x, int y) const {
	return Rectf(get_tile_position(x, y), get_tile_position(x + 1, y + 1));
}

Rect TileMap::get_tiles_overlapping(const Rectf& rect) const {
	Rectf m_rect = rect;
	m_rect.move(-m_offset);
	// log_warning << "overlap: " << m_rect << '\n';

	int t_left = std::max(0, static_cast<int>(std::floor(m_rect.get_left() / BLOCK_SIZE)));
	int t_top = std::max(0, static_cast<int>(std::floor(m_rect.get_top() / BLOCK_SIZE)));
	int t_right = std::min(m_width, static_cast<int>(std::ceil(m_rect.get_right() / BLOCK_SIZE)));
	int t_bottom = std::min(m_height, static_cast<int>(std::ceil(m_rect.get_bottom() / BLOCK_SIZE)));

	return Rect(t_left, t_top, t_right, t_bottom);
}

bool TileMap::is_outside_bounds(const Vector& pos) const {
	Vector m_pos = (pos - m_offset) / BLOCK_SIZE;
	float width = static_cast<float>(m_width);
	float height = static_cast<float>(m_height);
	return (m_pos.x < 0.0f || m_pos.x >= width || m_pos.y < 0.0f || m_pos.y >= height);
}

void TileMap::set_tileset(const TileSet* new_tileset) { m_tileset = new_tileset; }

void TileMap::parse(const std::string& filename) {
	ReaderMachine reader(filename);
	const ReaderData* data = reader.get_data(0);
	
	data->get("row", m_width);
	data->get("col", m_height);
	
	if (m_width <= 0 || m_height <= 0) {
		throw std::runtime_error("width/height unknown");
	}
	m_tiles.clear();
	m_tiles.resize(m_width * m_height);
	
	data->get("layer", m_layer);
	
	int solid = 0;
	data->get("solid", solid);
	m_is_solid = static_cast<bool>(solid);

	std::vector<std::vector<uint32_t>> board;
	data->get("board", board);
	
	if (static_cast<int>(board.size()) != m_height) {
		throw std::runtime_error("different with the number of rows defined (height)");
	}

	for (const auto& rows : board) {
		if (static_cast<int>(rows.size()) != m_width) {
			throw std::runtime_error("different with the number of rows defined (height)");
		}
	}

	for (int i = 0; i < m_height; ++ i) {
		std::move(board[i].begin(), board[i].end(), m_tiles.begin() + i * m_width);
	}
}

std::unique_ptr<TileMap> TileMap::clone(const Vector& offset) const {
	auto tilemap = std::make_unique<TileMap>(*this);
	tilemap->set_offset(offset);
	return tilemap;
}