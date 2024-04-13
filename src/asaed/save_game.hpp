#ifndef HEADER_ASAED_ASAED_SAVE_GAME_HPP
#define HEADER_ASAED_ASAED_SAVE_GAME_HPP

#include <memory>

class PlayerStatus;
class Player;

class SaveGame final {
private:
	std::unique_ptr<PlayerStatus> m_player_status;

public:
	SaveGame();
	~SaveGame();

private:
	SaveGame(const SaveGame&) = delete;
	SaveGame& operator=(const SaveGame&) = delete;

public:
	PlayerStatus& get_player_status() const;
};

#endif