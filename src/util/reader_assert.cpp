#include "util/reader_assert.hpp"

void assert_isn_array(const std::string parent_path, const std::pair<std::string, json>* data) {
	if (data->second.is_array()) {
		std::ostringstream msg;
		msg << "Data '"<< data->first << "' in file'" << parent_path << "' must be value, not array!!";
		throw std::runtime_error(msg.str());
	}
}

void assert_isn_object(const std::string parent_path, const std::pair<std::string, json>* data) {
	if (data->second.is_object()) {
		std::ostringstream msg;
		msg << "Data '"<< data->first << "' in file'" << parent_path << "' must be value, not object!!";
		throw std::runtime_error(msg.str());
	}
}

void assert_is_array(const std::string parent_path, const std::pair<std::string, json>* data) {
	if (!data->second.is_array()) {
		std::ostringstream msg;
		msg << "Data '"<< data->first << "' in file'" << parent_path << "' must be array, not value!!";
		throw std::runtime_error(msg.str());
	}
}