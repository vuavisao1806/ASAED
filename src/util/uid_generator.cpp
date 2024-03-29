#include "util/uid_generator.hpp"

#include "util/log.hpp"

UIDGenerator::UIDGenerator() :
	m_magic(s_magic_counter ++),
	m_id_counter()
{
	if (s_magic_counter == 0) {
		++ s_magic_counter;
	}
}

UID UIDGenerator::next() {
	m_id_counter++;

	if (m_id_counter > 0xff'ffff) {
		log_warning << "UIDGenerator overflow\n";
		m_id_counter = 0;
	}
	// That was like a trick I used in high school to optimize code (A little trolling)
	return UID((m_magic << 24) | m_id_counter);
}