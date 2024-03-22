#include "util/reader_machine.hpp"

#include "util/log.hpp"
#include "util/reader_data.hpp"

ReaderMachine::ReaderMachine(const std::string& filename) :
	m_is(filename),
	m_filename(filename),
	m_value()
{
	log_debug << "ReaderMachine::read: " << filename << '\n';
	if (!std::filesystem::exists(filename)) {
		std::ostringstream msg;
		msg << "File '" << filename << "' not exist";
		throw std::runtime_error(msg.str());
	}
	m_parent_name = std::filesystem::path(m_filename).parent_path().string() + '/';
	if (!m_is.is_open()) {
		std::ostringstream msg;
		msg << "Can't open file '" << filename << "'";
		throw std::runtime_error(msg.str());
	}
	m_is >> m_value;

	parse();
}

void ReaderMachine::parse() {
	log_debug << "ReaderMachine::parse: " << m_filename << '\n';
	if (m_value.size() != 1) {
		std::ostringstream msg;
		msg << "Big object read from file '" << m_filename << "' should be unique!!\n";
		throw std::runtime_error(msg.str());
	}
	else {
		json objects = m_value.begin().value();
		for (const auto& object : objects) {
			m_data_holder.push_back(std::make_unique<ReaderData>(m_parent_name));
			for (const auto& [key, val] : object.items()) {
				m_data_holder.back()->apply(make_pair(key, val));
			}
			// std::cout << "--------------------------\n"; // test
		}
	}
}

ReaderMachine::~ReaderMachine() {
	m_data_holder.clear();
}

std::string ReaderMachine::get_filename() const { return m_filename; }
std::string ReaderMachine::get_parent_name() const { return m_parent_name; }
const json& ReaderMachine::get_json_value() const { return m_value; }

size_t ReaderMachine::get_size() const { return m_data_holder.size(); }
const ReaderData* ReaderMachine::get_data(int idx) { return m_data_holder[idx].get(); }