#include "util/reader_iterator.hpp"

#include "util/reader_machine.hpp"

ReaderIterator::ReaderIterator(const ReaderMachine& reader) :
	m_reader(reader),
	m_idx(0),
	m_obj()
{
	const auto& value = m_reader.get_json_value();
	if (static_cast<int>(value.size()) != 1) {
		std::ostringstream msg;
		msg << "Big object read from file '" << m_reader.get_filename() << "' should be unique!!\n";
		throw std::runtime_error(msg.str());
	}
	else {
		json objects = value.begin().value();
		for (const auto& object : objects) {
			for (const auto& [key, val] : object.items()) {
				m_obj.push_back(make_pair(key, val));
			}
		}
	}
}

ReaderIterator::~ReaderIterator() {
	clear();
}

bool ReaderIterator::next() {
	++ m_idx;
	return (m_idx < m_obj.size());
}

const std::pair<std::string, json>* ReaderIterator::get_data(int idx) const { return &m_obj[idx]; }
size_t ReaderIterator::get_size() const { return m_obj.size(); }

std::string ReaderIterator::get_key() const { return m_obj[m_idx].first; }
const json& ReaderIterator::get_value() const { return m_obj[m_idx].second; }
const ReaderMachine& ReaderIterator::get_reader() const { return m_reader; }

void ReaderIterator::clear() {
	m_obj.clear();
}