#ifndef HEADER_ASAED_UTIL_READER_DATA_HPP
#define HEADER_ASAED_UTIL_READER_DATA_HPP

#include <string>
#include <tuple>
#include <memory>

#include "util/json.hpp"

using json = nlohmann::json;

class ReaderMachine;

class ReaderData final {
private:
	std::vector<std::pair<std::string, json>> m_object;
	
public:
	const std::string& m_parent_path;

private:
	ReaderData() = delete;

public:
	~ReaderData();

private:
	ReaderData(const ReaderData&) = delete;
	ReaderData& operator=(const ReaderData&) = delete;

public:
	ReaderData(const std::string& parent_name);

public:
	const std::pair<std::string, json>* get_item(const std::string name) const;

	bool get(const std::string& name, int& value) const;
	bool get(const std::string& name, uint32_t& value) const;
	bool get(const std::string& name, float& value) const;
	bool get(const std::string& name, std::string& value) const;

	bool get(const std::string& name, std::vector<int>& values) const;
	bool get(const std::string& name, std::vector<uint32_t>& values) const;
	bool get(const std::string& name, std::vector<float>& values) const;
	bool get(const std::string& name, std::vector<std::string>& values) const;

	bool get(const std::string& name, std::vector<std::vector<uint32_t>>& values) const;

	void apply(const std::pair<std::string, json>& data);
	void clear();
};

#endif