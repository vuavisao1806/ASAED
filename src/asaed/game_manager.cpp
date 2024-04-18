#include "asaed/game_manager.hpp"

#include "asaed/game_session.hpp"
#include "asaed/save_game.hpp"
#include "asaed/screen_manager.hpp"

GameManager::GameManager() :
	m_savegame(std::make_unique<SaveGame>())
{}

GameManager::~GameManager() {
	m_savegame.reset();
}

void GameManager::start_level(const std::string& filename_level, bool next) {
	if (next) {
		ScreenManager::current()->pop_screen();
	}
	if (filename_level == "endgame") {
		return;
	}
	ScreenManager::current()->push_screen(std::make_unique<GameSession>(filename_level, *m_savegame));
}
