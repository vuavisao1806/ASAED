#ifndef HEADER_ASAED_UTIL_READER_MACHINE_HPP
#define HEADER_ASAED_UTIL_READER_MACHINE_HPP

#include <fstream>
#include <memory>
#include <vector>
#include <string>

#include "util/json.hpp"

using json = nlohmann::json;

class ReaderData;

/**
 * ReaderMachine directly reads xyz.json, parses this, and stores data in m_data_holder to query this 
 * My pattern in xyz.json:
 * {
 * 		"big_object": [
 * 			"data_set_1": 
 * 			"data_set_2": 
 * 			.
 * 			.
 * 			.
 * 			.
 * 			"data_set_n": 
 * 		]
 * }
 * big_object should be unique
 * data_set can be int, float, string, array, ...
*/

class ReaderMachine final {
private:
	std::ifstream m_is;
	std::string m_filename;
	std::string m_parent_name;
	json m_value;

	std::vector<std::unique_ptr<ReaderData>> m_data_holder;
private:
	ReaderMachine() = delete;

public:
	~ReaderMachine();

private:
	// ReaderMachine(const ReaderMachine&) = delete;
	ReaderMachine& operator=(const ReaderMachine&) = delete;

public:
	ReaderMachine(const std::string& filename);

public:
	void parse();

public:
	std::string get_filename() const;
	std::string get_parent_name() const;
	const json& get_json_value() const;

	size_t get_size() const;
	const ReaderData* get_data(int idx);
};

#endif