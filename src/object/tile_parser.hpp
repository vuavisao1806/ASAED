#ifndef HEADER_ASAED_OBJECT_TILE_PARSE_HPP
#define HEADER_ASAED_OBJECT_TILE_PARSE_HPP

#include <string>

class ReaderData;
class TileSet;

/** TileSetParser helps read Tileset from xyz.json by pattern */

class TileSetParser final {
private:
	TileSet& m_tileset;
	std::string m_filename;

public:
	TileSetParser(TileSet& tileset, const std::string& name);

private:
	TileSetParser(const TileSetParser&) = delete;
	TileSetParser& operator=(const TileSetParser&) = delete;

public:
	void parse();
	void parse_tile(const ReaderData* data);
};

#endif