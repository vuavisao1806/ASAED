#ifndef HEADER_ASAED_ASAED_PLAYER_STATUS_HPP
#define HEADER_ASAED_ASAED_PLAYER_STATUS_HPP

#include <memory>

class Player;

class PlayerStatus final {
private:
	std::unique_ptr<Player> m_player;

public:
	PlayerStatus();
	~PlayerStatus();

private:
	PlayerStatus(const PlayerStatus&) = delete;
	PlayerStatus& operator=(const PlayerStatus&) = delete;

public:
	void reset_player_status();

	std::unique_ptr<Player> clone_player_status() const;
	void set_player_status(const Player& player);
};

#endif