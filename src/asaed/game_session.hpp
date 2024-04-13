#ifndef HEADER_ASAED_ASAED_GAME_SESSION_HPP
#define HEADER_ASAED_ASAED_GAME_SESSION_HPP

#include <memory>
#include <string>

#include "asaed/screen.hpp"
#include "util/currenton.hpp"

class Level;
class Compositor;
class Controller;
class DrawingContext;
class SaveGame;

class GameSession final : public Screen, public Currenton<GameSession> {
private:
	std::unique_ptr<Level> m_level;
	SaveGame& m_savegame;
	bool m_game_pause;

public:
	GameSession(const std::string& filename, SaveGame& savegame);
	~GameSession() override;

private:
	GameSession(const GameSession&) = delete;
	GameSession& operator=(const GameSession&) = delete;

public:
	virtual void draw(Compositor& compositor) override;
	virtual void update(float dt_sec, const Controller& controller) override;

	virtual void setup() override;
	virtual void leave() override;

	Level& get_current_level() const;
	SaveGame& get_savegame() const;
	
	void draw_pause(DrawingContext& drawing_context);
	void check_end_conditions();
	// void finish(bool win = true);

public:

};

#endif