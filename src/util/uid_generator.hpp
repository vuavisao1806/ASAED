#ifndef HEADER_ASAED_UTIL_UID_GENERATOR_HPP
#define HEADER_ASAED_UTIL_UID_GENERATOR_HPP

#include "util/uid.hpp"

class UIDGenerator {
private:
	static uint8_t s_magic_counter;

private:
	uint8_t m_magic;
	uint32_t m_id_counter;

public:
	UIDGenerator();

private:
	UIDGenerator(const UIDGenerator&) = delete;
	UIDGenerator& operator=(const UIDGenerator&) = delete;

public:
	UID next();
};

#endif