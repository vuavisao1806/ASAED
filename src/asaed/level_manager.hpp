#ifndef HEADER_ASAED_ASAED_LEVEL_MANAGER_HPP
#define HEADER_ASAED_ASAED_LEVEL_MANAGER_HPP

#include "asaed/level_manager.hpp"

#include <map>
#include <memory>
#include <string>

#include "util/currenton.hpp"

class ReaderData;
class LevelData;

class LevelManager final : public Currenton<LevelManager> {
	friend class LevelData;
private:
	std::string m_filename;
	std::map<std::string, std::unique_ptr<LevelData>> m_levels;

public:
	LevelManager();
	~LevelManager() override;

private:
	LevelManager(const LevelManager&) = delete;
	LevelManager& operator=(const LevelManager&) = delete;

private:
	void parse_level(const ReaderData* data);

public:
	const LevelData& get(const std::string& filename) const;
};

#endif