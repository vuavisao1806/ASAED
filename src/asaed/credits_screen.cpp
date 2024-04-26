#include "asaed/credits_screen.hpp"

#include "asaed/game_session.hpp"
#include "audio/sound_manager.hpp"
#include "control/controller.hpp"
#include "object/scroller_text.hpp"
#include "video/canvas.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"

CreditsScreen::CreditsScreen() :
	m_text_scroller(std::make_unique<ScrollerText>("credits.json"))
{}

CreditsScreen::~CreditsScreen()
{}

void CreditsScreen::draw(Compositor& compositor) {
	auto& drawing_context = compositor.make_context();
	m_text_scroller->draw(drawing_context);
}

void CreditsScreen::update(float dt_sec, const Controller& /* controller */) {
	SoundManager::current()->play_music("Estavius.mp3");
	m_text_scroller->update(dt_sec);
}

void CreditsScreen::setup() {}
void CreditsScreen::leave() {}