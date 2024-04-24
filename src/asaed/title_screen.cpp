#include "asaed/title_screen.hpp"

#include "asaed/game_session.hpp"
#include "audio/sound_manager.hpp"
#include "control/controller.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_storage.hpp"
#include "video/canvas.hpp"
#include "video/compositor.hpp"
#include "video/layer.hpp"
#include "video/drawing_context.hpp"
#include "video/surface.hpp"

TitleScreen::TitleScreen() :
	m_theme(Surface::from_file("images/menu/theme.png"))
{}

TitleScreen::~TitleScreen()
{}

void TitleScreen::draw(Compositor& compositor) {
	auto& drawing_context = compositor.make_context();
	drawing_context.get_canvas().draw_surface_scaled(m_theme, drawing_context.get_viewport(), Color(1.0f, 1.0f, 1.0f), LAYER_GUI - 50);
	
	
	// MouseCursor::current()->draw(drawing_context); // temporary
}

void TitleScreen::update(float /* dt_sec */, const Controller& /* controller */) {
	SoundManager::current()->play_music("Estavius.mp3");

	if (!MenuManager::current()->is_active() && !GameSession::current()) {
		MenuManager::current()->set_menu(MenuStorage::MAIN_MENU);
	}
}

void TitleScreen::setup() {}
void TitleScreen::leave() {}