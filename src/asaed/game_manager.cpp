#include "asaed/game_manager.hpp"

#include "asaed/game_session.hpp"
#include "asaed/room.hpp"
#include "asaed/save_game.hpp"
#include "asaed/screen_manager.hpp"
#include "asaed/player_status.hpp"
#include "object/player.hpp"

GameManager::GameManager() :
	m_savegame(std::make_unique<SaveGame>())
{}

GameManager::~GameManager() {
	m_savegame.reset();
}

void GameManager::start_level(const std::string& filename_level, bool next) {
	if (next) {
		m_savegame->get_player_status().set_player_status(*Room::get().get_player()[0]);
		ScreenManager::current()->pop_screen();
	}
	else {
		m_savegame->get_player_status().reset_player_status();
	}
	if (filename_level == "endgame") {
		return;
	}
	ScreenManager::current()->push_screen(std::make_unique<GameSession>(filename_level, *m_savegame));
}
