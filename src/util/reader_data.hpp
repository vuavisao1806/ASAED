#ifndef HEADER_ASAED_UTIL_READER_DATA_HPP
#define HEADER_ASAED_UTIL_READER_DATA_HPP

#include <string>
#include <tuple>
#include <memory>

#include "util/json.hpp"

using json = nlohmann::json;

class ReaderMachine;
class ReaderIterator;

class ReaderData final {
private:
	std::string m_data;
	std::unique_ptr<ReaderMachine> m_reader;
	std::unique_ptr<ReaderIterator> m_data_holder;

private:
	ReaderData() = delete;

private:
	ReaderData(const ReaderData&) = delete;
	ReaderData& operator=(const ReaderData&) = delete;

public:
	ReaderData(const std::string& data);

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

	const ReaderMachine& get_reader() const;
	const ReaderIterator& get_data_holder() const;
};

#endif