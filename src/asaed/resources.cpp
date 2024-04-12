#include "asaed/resources.hpp"

#include "gui/mouse_cursor.hpp"
#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"

std::unique_ptr<MouseCursor> Resources::m_mouse_cursor;

Resources::Resources() {
	load();
}

Resources::~Resources() {
	unload();
}


void Resources::load() {
	m_mouse_cursor = std::make_unique<MouseCursor>(SpriteManager::current()->create("data/images/menu/mouse-cursor.json"));
}

void Resources::unload() {
	m_mouse_cursor.reset();
}