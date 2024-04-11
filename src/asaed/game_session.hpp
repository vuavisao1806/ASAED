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

class GameSession final : public Screen, public Currenton<GameSession> {
private:
	std::unique_ptr<Level> m_level;
	bool m_game_pause;

public:
	GameSession(const std::string& filename);
	~GameSession() override;

public:
	virtual void draw(Compositor& compositor) override;
	virtual void update(float dt_sec, const Controller& controller) override;

	virtual void setup() override;
	virtual void leave() override;

	Level& get_current_level() const;
	void draw_pause(DrawingContext& drawing_context);
	// void check_end_conditions(); will appear soon
	// void finish(bool win = true); will appear soon
	// void start_next_level(); will appear soon
private:
	GameSession(const GameSession&) = delete;
	GameSession& operator=(const GameSession&) = delete;

public:

};

#endif