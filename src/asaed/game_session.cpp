#include "asaed/game_session.hpp"

#include "asaed/level.hpp"
#include "asaed/level_data.hpp"
#include "asaed/level_manager.hpp"
#include "audio/sound_manager.hpp"
#include "control/controller.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"
#include "video/layer.hpp"


GameSession::GameSession(const std::string& filename) :
	m_level(),
	m_game_pause(false)
{
	m_level = LevelManager::current()->get(filename).creat_level_from_leveldata();
}

GameSession::~GameSession()
{}

void GameSession::draw(Compositor& compositor) {
	auto& drawing_context = compositor.make_context();

	m_level->draw(drawing_context);

	if (m_game_pause) {
		draw_pause(drawing_context);
	}
}

void GameSession::draw_pause(DrawingContext& drawing_context) {
	drawing_context.get_canvas().draw_filled_rect(drawing_context.get_rect(), 
	                                              Color(0.0f, 0.0f, 0.0f, 0.25f), 
	                                              LAYER_FOREGROUND);
}

void GameSession::update(float dt_sec, const Controller& /* controller */) {
	if (!m_game_pause) {
		m_level->update(dt_sec);
	}
}

void GameSession::setup() {
	m_level->start_level();
	SoundManager::current()->play_music("boss.mp3");
}

void GameSession::leave() {

}

Level& GameSession::get_current_level() const { return *m_level.get(); }