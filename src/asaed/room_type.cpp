#include "asaed/room_type.hpp"

RoomType from_string_to_room_type(const std::string& name) {
	/** Why switch-case doesn't accept string!! */
	if (name == "normal") return RoomType::NORMAL;
	if (name == "start") return RoomType::START;
	if (name == "end") return RoomType::END;
	if (name == "bridge") return RoomType::BRIDGE;
	throw std::runtime_error("Unknown type of room!!");
	return RoomType::NORMAL;
}