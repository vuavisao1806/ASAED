#include "asaed/player_status.hpp"

#include "object/player.hpp"

PlayerStatus::PlayerStatus() :
	m_player()
{}

PlayerStatus::~PlayerStatus() {
	m_player.reset();
}

void PlayerStatus::reset_player_status() { m_player = std::make_unique<Player>(0, 1); }
void PlayerStatus::set_player_status(const Player& player) { m_player = std::move(player.clone()); }
std::unique_ptr<Player> PlayerStatus::clone_player_status() const { return m_player->clone(); }
