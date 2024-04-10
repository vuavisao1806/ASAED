#ifndef HEADER_ASAED_ASAED_LEVEL_DATA_HPP
#define HEADER_ASAED_ASAED_LEVEL_DATA_HPP

#include <memory>
#include <vector>
#include <string>

#include "object/direction.hpp"
#include "math/vector.hpp"

class TileSet;
class RoomAnalysis;
class Level;
class TileMap;
class Room;

class LevelData final {
	friend class Room;
	friend class LevelManager;
	friend class RoomAnalysis;
private:
	const TileSet* m_tileset;
	std::vector<std::unique_ptr<RoomAnalysis>> m_rooms;
	std::string m_next_level;

public:
	LevelData();
	~LevelData();

private:
	LevelData(const LevelData&) = delete;
	LevelData& operator=(const LevelData&) = delete;

public:
	void add_room(uint32_t id, std::unique_ptr<RoomAnalysis> room);
	const RoomAnalysis& get(const uint32_t id) const;
	size_t get_num_rooms() const;

	void set_next_level(const std::string& next_level);

	std::unique_ptr<Level> creat_level_from_leveldata() const;

	std::unique_ptr<Room> creat_room(std::unique_ptr<RoomAnalysis> room_analysis, const std::unique_ptr<Room>& prev = nullptr) const;
	std::unique_ptr<Room> creat_bridge(const std::unique_ptr<Room>& prev) const;
};

std::vector<Direction> get_next(const std::unique_ptr<Room>& prev);
Vector get_offset(const std::unique_ptr<Room>& prev, const std::unique_ptr<TileMap>& tilemap);
Direction inverse_direction(Direction direction);

#endif