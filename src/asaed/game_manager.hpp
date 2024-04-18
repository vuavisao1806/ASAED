#ifndef HEADER_ASAED_ASAED_GAME_MANAGER_HPP
#define HEADER_ASAED_ASAED_GAME_MANAGER_HPP

#include <memory>
#include <string>

#include "util/currenton.hpp"

class SaveGame;

class GameManager final : public Currenton<GameManager> {
private:
	std::unique_ptr<SaveGame> m_savegame;

public:
	GameManager();
	~GameManager() override;

private:
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

public:
	void start_level(const std::string& filename_level, bool next);
};

#endif