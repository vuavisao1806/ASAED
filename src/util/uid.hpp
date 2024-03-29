#ifndef HEADER_ASAED_UTIL_UID_HPP
#define HEADER_ASAED_UTIL_UID_HPP

#include <assert.h>
#include <iosfwd>
#include <stdint.h>
#include <functional>

class UID;

namespace std {
	template<>
	struct hash<UID> {
		size_t operator()(const UID& uid) const;
	};
} // namespace std

class UID {
	friend class UIDGenerator;
protected:
	uint32_t m_value;

public:
	UID();

private:
	explicit UID(uint32_t value);

public:
	UID(const UID&) = default;
	UID& operator=(const UID&) = default;

public:
	operator bool() const;

	bool operator<(const UID& other) const;
	bool operator==(const UID& other) const;
	bool operator!=(const UID& other) const;

public:
	friend size_t std::hash<UID>::operator()(const UID&) const;
	friend std::ostream& operator<<(std::ostream& os, const UID& uid);
};

std::ostream& operator<<(std::ostream& os, const UID& uid);

#endif