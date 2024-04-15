#include "asaed/game_session.hpp"

#include "asaed/level.hpp"
#include "asaed/level_data.hpp"
#include "asaed/level_manager.hpp"
#include "asaed/room.hpp"
#include "asaed/screen_manager.hpp"
#include "audio/sound_manager.hpp"
#include "asaed/save_game.hpp"
#include "control/controller.hpp"
#include "gui/mouse_cursor.hpp"
#include "gui/menu_storage.hpp"
#include "gui/menu_manager.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"
#include "video/layer.hpp"


GameSession::GameSession(const std::string& filename, SaveGame& savegame) :
	m_level(LevelManager::current()->get(filename).creat_level_from_leveldata()),
	m_savegame(savegame),
	m_game_pause(false)
{}

GameSession::~GameSession()
{}

void GameSession::draw(Compositor& compositor) {
	auto& drawing_context = compositor.make_context();

	m_level->draw(drawing_context);

	if (m_game_pause) {
		draw_pause(drawing_context);
	}
	
	MouseCursor::current()->draw(drawing_context);
}

void GameSession::update(float dt_sec, const Controller& controller) {
	if (controller.pressed(Control::ESCAPE)) {
		toggle_pause();
	}

	if (!m_game_pause) {
		m_level->update(dt_sec);
	}

	// unpause
	if (m_game_pause && !MenuManager::current()->is_active()) {
		m_game_pause = false;
	}

	check_end_conditions();
}

void GameSession::draw_pause(DrawingContext& drawing_context) {
	drawing_context.get_canvas().draw_filled_rect(drawing_context.get_rect(), 
	                                              Color(0.0f, 0.0f, 0.0f, 0.25f), 
	                                              LAYER_FOREGROUND);
}

void GameSession::check_end_conditions() {
	// if (Room::get().get_player().empty()) {
	// 	ScreenManager::current()->pop_screen();
	// }
}

void GameSession::setup() {
	m_level->start_level();
	SoundManager::current()->play_music("boss.mp3");
}

void GameSession::leave() {}

SaveGame& GameSession::get_savegame() const { return m_savegame; }
Level& GameSession::get_current_level() const { return *m_level.get(); }

void GameSession::toggle_pause() {
	if (!m_game_pause && !MenuManager::current()->is_active()) {
		MenuManager::current()->set_menu(MenuStorage::GAME_MENU);
		m_game_pause = true;
	}
}

void GameSession::quit_game() {
	MenuManager::current()->clear_menu_stack();
	ScreenManager::current()->pop_screen();
	
}
