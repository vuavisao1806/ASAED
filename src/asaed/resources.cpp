#include "asaed/resources.hpp"

#include "gui/mouse_cursor.hpp"
#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"
#include "video/ttf_font.hpp"
#include "video/surface.hpp"

std::unique_ptr<MouseCursor> Resources::m_mouse_cursor;

TTFFontPtr Resources::normal_font;
TTFFontPtr Resources::small_font;
TTFFontPtr Resources::big_font;

SurfacePtr Resources::arrow_left;
SurfacePtr Resources::arrow_right;
SurfacePtr Resources::back;

SurfacePtr Resources::heart;
SurfacePtr Resources::shield;

Resources::Resources() {
	load();
}

Resources::~Resources() {
	unload();
}


void Resources::load() {
	m_mouse_cursor = std::make_unique<MouseCursor>(SpriteManager::current()->create("data/images/menu/mouse-cursor.json"));

	normal_font = std::make_shared<TTFFont>("data/fonts/SuperMario256.ttf", 18, 1.5f);
	small_font = std::make_shared<TTFFont>("data/fonts/SuperMario256.ttf", 11, 1.5f);
	big_font = std::make_shared<TTFFont>("data/fonts/SuperMario256.ttf", 22, 1.5f);

	arrow_left = Surface::from_file("data/images/menu/arrow-left.png");
	arrow_right = Surface::from_file("data/images/menu/arrow-right.png");
	back = Surface::from_file("data/images/menu/arrow-back.png");

	heart = Surface::from_file("data/images/menu/HUD/heart.png");
	shield = Surface::from_file("data/images/menu/HUD/shield.png");
}

void Resources::unload() {
	m_mouse_cursor.reset();

	normal_font.reset();
	small_font.reset();
	big_font.reset();

	arrow_left.reset();
	arrow_right.reset();
	back.reset();
}