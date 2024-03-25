#include "util/reader_data.hpp"

// #include <iostream> // test

#include "util/reader_assert.hpp"
#include "util/reader_machine.hpp"

ReaderData::~ReaderData() {
	clear();
}

ReaderData::ReaderData(const std::string& parent_name) :
	m_parent_path(parent_name)
{}

const std::pair<std::string, json>* ReaderData::get_item(const std::string name) const {
	for (const auto& it : m_object) {
		if (it.first == name) {
			return &it;
		}
	}
	return nullptr;
}

#define GET_VALUE_MARCO(type)                     \
	const auto& data = get_item(name);            \
	if (data == nullptr) {                        \
		return false;                             \
	}                                             \
	else {                                        \
		assert_isn_array(m_parent_path, data);    \
		assert_isn_object(m_parent_path, data);   \
		value = data->second.get<type>();         \
		return true;                              \
	}


bool ReaderData::get(const std::string& name, int& value) const {
	GET_VALUE_MARCO(int);
}

bool ReaderData::get(const std::string& name, uint32_t& value) const {
	GET_VALUE_MARCO(uint32_t);
}

bool ReaderData::get(const std::string& name, float& value) const {
	GET_VALUE_MARCO(float);
}

bool ReaderData::get(const std::string& name, std::string& value) const {
	GET_VALUE_MARCO(std::string);
}

#undef GET_VALUE_MARCO

#define GET_VALUE_MARCO(type)                     \
	const auto& data = get_item(name);            \
	if (data == nullptr) {                        \
		return false;                             \
	}                                             \
	else {                                        \
		assert_is_array(m_parent_path, data);     \
		values.clear();                           \
		values = data->second.get<type>();        \
		return true;                              \
	}

bool ReaderData::get(const std::string& name, std::vector<int>& values) const {
	GET_VALUE_MARCO(std::vector<int>);
}

bool ReaderData::get(const std::string& name, std::vector<uint32_t>& values) const {
	GET_VALUE_MARCO(std::vector<uint32_t>);
}

bool ReaderData::get(const std::string& name, std::vector<float>& values) const {
	GET_VALUE_MARCO(std::vector<float>);
}

bool ReaderData::get(const std::string& name, std::vector<std::string>& values) const {
	GET_VALUE_MARCO(std::vector<std::string>);
}

bool ReaderData::get(const std::string& name, std::vector<std::vector<uint32_t>>& values) const {
	GET_VALUE_MARCO(std::vector<std::vector<uint32_t>>);
}

#undef GET_VALUE_MARCO

void ReaderData::apply(const std::pair<std::string, json>& data) {
	m_object.push_back(data);
	// std::cout << data.first << ' ' << data.second << '\n'; // test
}

void ReaderData::clear() {
	m_object.clear();
}
