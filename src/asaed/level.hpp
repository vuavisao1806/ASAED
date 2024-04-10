#ifndef HEADER_ASAED_ASAED_LEVEL_HPP
#define HEADER_ASAED_ASAED_LEVEL_HPP

#include <memory>
#include <string>
#include <vector>
#include <stdint.h>

class Room;
class DrawingContext;

class Level final {
	friend class Room;
private:
	std::vector<std::unique_ptr<Room>> m_rooms;
	std::string m_next_level;

public:
	Level();
	~Level();

private:
	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;

public:
	void update(float dt_sec);
	void draw(DrawingContext& drawing_context);

	void start_level();
	void add_room(std::unique_ptr<Room> room);

	void set_next_level(const std::string& next_level);
	const std::string& get_next_level();

	const std::unique_ptr<Room>& get(uint32_t id);
	size_t get_num_rooms() const;
};

#endif