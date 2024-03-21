#ifndef HEADER_ASAED_UTIL_READER_ITERATOR_HPP
#define HEADER_ASAED_UTIL_READER_ITERATOR_HPP

#include <tuple>
#include <vector>
#include <string>

#include "util/json.hpp"

using json = nlohmann::json;

class ReaderMachine;

class ReaderIterator final {
private:
	const ReaderMachine& m_reader;
	size_t m_idx;
	/** to hold all [key, value] like as map */
	std::vector<std::pair<std::string, json>> m_obj;

private:
	ReaderIterator() = delete;

public:
	~ReaderIterator();

private:
	ReaderIterator(const ReaderIterator&) = delete;
	ReaderIterator& operator=(const ReaderIterator&) = delete;

public:
	ReaderIterator(const ReaderMachine& reader);

public:
	bool next();

	const std::pair<std::string, json>* get_data(int idx) const;
	size_t get_size() const;

	std::string get_key() const;
	const json& get_value() const;
	const ReaderMachine& get_reader() const;

	void clear();
};

#endif