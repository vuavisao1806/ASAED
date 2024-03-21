#include "util/reader_data.hpp"

#include "util/reader_assert.hpp"
#include "util/reader_iterator.hpp"
#include "util/reader_machine.hpp"

ReaderData::ReaderData(const std::string& data) :
	m_data(data)
{
	m_reader = std::make_unique<ReaderMachine>(data);
	m_data_holder = std::make_unique<ReaderIterator>(*m_reader.get());
}

const std::pair<std::string, json>* ReaderData::get_item(const std::string name) const {
	for (size_t i = 0; i < m_data_holder->get_size(); ++ i) {
		const auto& m_data = m_data_holder->get_data(i);
		if (m_data->first == name) {
			return m_data;
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
		assert_isn_array(data->second);           \
		assert_isn_object(data->second);          \
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

#define GET_VALUE_MARCO()                         \
	const auto& data = get_item(name);            \
	if (data == nullptr) {                        \
		return false;                             \
	}                                             \
	else {                                        \
		assert_is_array(data->second);            \
		values.clear();                           \
		for (const auto& value : data->second) {  \
			values.push_back(value);              \
		}                                         \
		return true;                              \
	}

bool ReaderData::get(const std::string& name, std::vector<int>& values) const {
	GET_VALUE_MARCO();
}

bool ReaderData::get(const std::string& name, std::vector<uint32_t>& values) const {
	GET_VALUE_MARCO();
}

bool ReaderData::get(const std::string& name, std::vector<float>& values) const {
	GET_VALUE_MARCO();
}
bool ReaderData::get(const std::string& name, std::vector<std::string>& values) const {
	GET_VALUE_MARCO();
}

const ReaderMachine& ReaderData::get_reader() const { return *m_reader; }
const ReaderIterator& ReaderData::get_data_holder() const { return *m_data_holder; }
