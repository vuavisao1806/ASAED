#include "util/uid.hpp"

#include <ostream>

namespace std {
	size_t hash<UID>::operator()(const UID& uid) const {
		return static_cast<size_t>(uid.m_value);
	}
}

UID::UID() :
	m_value()
{}

UID::operator bool() const {
	return (m_value != 0);
}

bool UID::operator<(const UID& other) const { return m_value < other.m_value; }
bool UID::operator==(const UID& other) const { return m_value == other.m_value; }
bool UID::operator!=(const UID& other) const { return m_value != other.m_value; }

std::ostream& operator<<(std::ostream& os, const UID& uid) {
	return os << uid.m_value;
}