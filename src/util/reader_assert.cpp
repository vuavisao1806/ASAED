#include "util/reader_assert.hpp"

void assert_isn_array(const json& data) {
	if (data.is_array()) {
		std::ostringstream msg;
		msg << "Data must be value, not array!!";
		throw std::runtime_error(msg.str());
	}
}

void assert_isn_object(const json& data) {
	if (data.is_object()) {
		std::ostringstream msg;
		msg << "Data must be value, not object!!";
		throw std::runtime_error(msg.str());
	}
}

void assert_is_array(const json& data) {
	if (!data.is_array()) {
		std::ostringstream msg;
		msg << "Data must be array, not value!!";
		throw std::runtime_error(msg.str());
	}
}