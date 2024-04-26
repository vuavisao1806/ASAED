#include "badguy/badguy_manager.hpp"

#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "util/log.hpp"
#include "badguy/anubis.hpp"
#include "badguy/badguy.hpp"
#include "badguy/ogre.hpp"
#include "badguy/wizzard.hpp"
#include "badguy/zulan.hpp"
#include "badguy/orc.hpp"
#include "badguy/necromancer.hpp"

BadGuyManager::BadGuyManager() :
	m_filename("images/badguy/badguy-set.json"),
	m_badguys()
{
	ReaderMachine reader(m_filename);
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse_badguy(reader.get_data(i));
	}
}

BadGuyManager::~BadGuyManager() {
	m_badguys.clear();
}

const BadGuy& BadGuyManager::get(const std::string& filename) const {
	auto it = m_badguys.find(filename);
	if (it == m_badguys.end()) {
		throw std::runtime_error("Undefined badguy");
	}
	return *it->second.get();
}


void BadGuyManager::parse_badguy(const ReaderData* data) {
	int id;
	if (!data->get("id", id)) {
		throw std::runtime_error("Missing badguy id");
	}

	std::string name;
	if (!data->get("name", name)) {
		throw std::runtime_error("Missing badguy name");
	}

	std::unique_ptr<BadGuy> badguy;
	switch (static_cast<BadGuyData>(id)) {
		case OGRE:
			badguy = Ogre::from_file(data);
			break;

		case WIZZARD:
			badguy = Wizzard::from_file(data);
			break;

		case ZULAN:
			badguy = Zulan::from_file(data);
			break;

		case ANUBIS:
			badguy = Anubis::from_file(data);
			break;
		
		case ORC:
			badguy = Orc::from_file(data);
			break;
		
		case NECROMANCER:
			badguy = Necromancer::from_file(data);
			break;
		
		default:
			throw std::runtime_error("Unknow bad guy");
			break;
	}
	m_badguys[name] = std::move(badguy);
}