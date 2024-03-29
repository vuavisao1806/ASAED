#ifndef HEADER_ASAED_OBJECT_TILE_MAP_HPP
#define HEADER_ASAED_OBJECT_TILE_MAP_HPP

#include <memory>
#include <vector>
#include <stdint.h>

#include "math/vector.hpp"
#include "math/size.hpp"
#include "math/rectf.hpp"
#include "object/game_object.hpp"

class TileSet;
class Tile;
class DrawingContext;

/**
 * TileMap stores its tileset for its map and every information about the current map 
 * Contains parses and draws a map
*/

class TileMap final : public GameObject {
private:
	const TileSet* m_tileset;

	using Tiles = std::vector<uint32_t>;
	Tiles m_tiles;

	bool m_is_solid; // (??)
	int m_width;
	int m_height;
	int m_layer;

	Vector m_offset;

public:
	~TileMap();

public:
	TileMap(const TileSet* tileset);
	TileMap(const TileSet* tileset, const std::string& filename);

private:
	TileMap(const TileMap&) = delete;
	TileMap& operator=(const TileMap&) = delete;

// public:
// 	static std::unique_ptr<TileMap> fromfile(const TileSet* tileset, const std::string& filename);

public:
	virtual void update(float /* dt_sec */) override; // finally
	virtual void draw(DrawingContext& context) override; // finally

	int get_width() const;
	int get_height() const;
	Size get_size() const;

	bool is_solid() const; // (??)

	void set_offset(const Vector& offset);
	Vector get_offset() const;

	void resize(int newWidth, int newHeight);

	/** get indirect (not care m_offset) */
	uint32_t get_tile_id(int x, int y) const;
	
	/** get direct (care m_offset) */
	uint32_t get_tile_id_at(const Vector& pos) const;

	/** get indirect (not care m_offset) */
	const Tile& get_tile(int x, int y) const;
	
	/** get direct (care m_offset) */
	const Tile& get_tile_at(const Vector& pos) const;

	/** change indirect (not care m_offset) */
	void change(int x, int y, uint32_t new_tile);
	
	/** change direct (care m_offset) */
	void change_at(const Vector& pos, uint32_t new_tile);

	/** return upper-left corner of tile (x, y) in real map */
	Vector get_tile_position(int x, int y) const;

	/** return real map */
	Rectf get_bounding_box() const;
	/** return bounding box in real map when upper-left is (x, y) */
	Rectf get_tile_bounding_box(int x, int y) const;
	
	/** return rect() filled by indices that overlap the given rectangle */
	Rect get_tiles_overlapping(const Rectf& rect) const;

	bool is_outside_bounds(const Vector& pos) const;

	void set_tileset(const TileSet* new_tileset);
	/** read map */
	void parse(const std::string& filename);
};

#endif