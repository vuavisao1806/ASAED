#ifndef HEADER_ASAED_WEAPON_MOVING_TILE_MOVING_TILE_SET_HPP
#define HEADER_ASAED_WEAPON_MOVING_TILE_MOVING_TILE_SET_HPP

#include "util/currenton.hpp"

#include <memory>
#include <vector>
#include <string>

class ReaderData;
class MovingTile;

class MovingTileSet final : public Currenton<MovingTileSet> {
	std::string m_filename;
	std::vector<std::unique_ptr<MovingTile>> m_movingtiles;

public:
	MovingTileSet();
	~MovingTileSet() override;

private:
	MovingTileSet(const MovingTileSet&) = delete;
	MovingTileSet& operator=(const MovingTileSet&) = delete;
	
public:
	void add_movingtile(uint32_t id, std::unique_ptr<MovingTile> weapon);
	const MovingTile& get(const uint32_t id) const;

public:
	void parse();
	void parse_movingtile(const ReaderData* data);
};

#endif