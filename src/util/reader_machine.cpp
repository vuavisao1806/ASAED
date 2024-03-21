#include "util/reader_machine.hpp"

#include "util/log.hpp"

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
}

ReaderMachine::~ReaderMachine()
{}

std::string ReaderMachine::get_filename() const { return m_filename; }
std::string ReaderMachine::get_parent_name() const { return m_parent_name; }
const json& ReaderMachine::get_json_value() const { return m_value; }
