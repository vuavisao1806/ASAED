#include "sprite/sprite_manager.hpp"

#include <sstream>

#include "sprite/sprite.hpp"
#include "sprite/sprite_data.hpp"

SpriteManager::SpriteManager() :
	sprites()
{}

SpriteManager::~SpriteManager() {
	sprites.clear();
}

SpriteData* SpriteManager::load(const std::string& filename) {
	auto data = std::make_unique<SpriteData>(filename);
	sprites[filename] = std::move(data);
	return sprites[filename].get();
}

SpritePtr SpriteManager::create(const std::string& filename) {
	auto it = sprites.find(filename);
	SpriteData* data;
	if (it == sprites.end()) {
		data = load(filename);
		if (data == nullptr) {
			std::ostringstream msg;
			msg << "Error loading sprite '" << filename << "'. The file cannot be found";
			throw std::runtime_error(msg.str());
		}
	}
	else {
		data = it->second.get();
	}
	return std::make_unique<Sprite>(*data);
}