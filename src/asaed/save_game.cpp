#include "asaed/save_game.hpp"

#include "asaed/player_status.hpp"

SaveGame::SaveGame() :
	m_player_status(std::make_unique<PlayerStatus>())
{
	m_player_status->reset_player_status();
}

SaveGame::~SaveGame() {
	m_player_status.reset();
}

PlayerStatus& SaveGame::get_player_status() const { return *m_player_status.get(); }
