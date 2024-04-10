#ifndef HEADER_ASAED_ASAED_ROOM_TYPE_HPP
#define HEADER_ASAED_ASAED_ROOM_TYPE_HPP

#include <string>
#include <stdexcept>

enum class RoomType {
	NORMAL,
	START,
	END,
	BRIDGE
};

RoomType from_string_to_room_type(const std::string& name);

#endif