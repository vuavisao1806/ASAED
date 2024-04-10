#ifndef HEADER_ASAED_ASAED_ROOM_ANALYSIS_HPP
#define HEADER_ASAED_ASAED_ROOM_ANALYSIS_HPP

#include "asaed/room_type.hpp"

#include <memory>
#include <vector>
#include <string>
#include <tuple>

#include "math/vector.hpp"

class TileMap;
class TileSet;
class ReaderData;
class Room;

class RoomAnalysis {
	friend class Room;
	friend class LevelData;
private:
	Room* m_parent;

	RoomType m_room_type;
	std::unique_ptr<TileMap> m_tilemap;

	/** Actually, the first purpose isn't like that, but :(( */
	std::vector<std::string> m_badguys;
	int m_turns;
	int m_min_turn_spawn_badguy;
	int m_max_turn_spawn_badguy;

public:
	RoomAnalysis();
	RoomAnalysis(RoomType room_type, std::unique_ptr<TileMap> tilemap);
	RoomAnalysis(const RoomAnalysis& other);
	~RoomAnalysis();

private:
	RoomAnalysis& operator=(const RoomAnalysis&) = delete;

public:
	static std::unique_ptr<RoomAnalysis> fromfile(const TileSet* tileset, const ReaderData* data);

public:
	void update();

	std::unique_ptr<RoomAnalysis> clone(const Vector& offset) const;

	void set_room_parent(Room* parent);

	void start_next_turn();
	bool is_turn_clear();
	bool is_room_clear();

	void set_offset(const Vector& offset);
	RoomType get_room_type() const;
};

#endif