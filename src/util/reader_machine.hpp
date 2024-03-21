#ifndef HEADER_ASAED_UTIL_READER_MACHINE_HPP
#define HEADER_ASAED_UTIL_READER_MACHINE_HPP

#include <fstream>
#include <string>

#include "util/json.hpp"

using json = nlohmann::json;

class ReaderMachine final {
private:
	std::ifstream m_is;
	std::string m_filename;
	std::string m_parent_name;
	json m_value;

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
	std::string get_filename() const;
	std::string get_parent_name() const;
	const json& get_json_value() const;
};

#endif