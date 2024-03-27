#include "object/tile_parser.hpp"

#include "object/tile.hpp"
#include "object/tile_set.hpp"
#include "util/log.hpp"
#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "video/surface.hpp"

TileSetParser::TileSetParser(TileSet& tileset, const std::string& name) :
	m_tileset(tileset),
	m_filename(name)
{}


void TileSetParser::parse() {
	ReaderMachine reader(m_filename);
	
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse_tile(reader.get_data(i));
	}
}

void TileSetParser::parse_tile(const ReaderData* data) {
	uint32_t id;
	if (!data->get("id", id)) {
		throw std::runtime_error("Missing tile id");
	}

	uint32_t attributes = 0;
	int value = 0;
	if (data->get("solid", value) && value) attributes |= Tile::SOLID;
	if (data->get("un-solid", value) && value) attributes |= Tile::UNSOLID;
	
	float fps = 1.0f;
	data->get("fps", fps);
	
	std::vector<std::string> image_filenames;
	if (!data->get("images", image_filenames)) {
		throw std::runtime_error("Tile doesn't have images !!");
	}
	std::vector<SurfacePtr> surfaces;
	for (const auto& filename : image_filenames) {
		SurfacePtr surface = Surface::from_file(data->m_parent_path + filename);
		surfaces.push_back(std::move(surface));
	}

	std::unique_ptr<Tile> tile = std::make_unique<Tile>(surfaces, attributes, fps);
	m_tileset.add_tile(id, std::move(tile));
}