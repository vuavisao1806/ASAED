#ifndef HEADER_ASAED_SPRITE_SPRITE_MANAGER_HPP
#define HEADER_ASAED_SPRITE_SPRITE_MANAGER_HPP

#include <map>
#include <memory>
#include <string>

#include "sprite/sprite_ptr.hpp"
#include "util/currenton.hpp"

class SpriteData;

class SpriteManager final : public Currenton<SpriteManager> {
private:
	std::map<std::string, std::unique_ptr<SpriteData>> sprites;

public:
	SpriteManager();
	~SpriteManager() override;

private:
	SpriteManager(const SpriteManager&) = delete;
	SpriteManager& operator=(const SpriteManager&) = delete;

private:
	SpriteData* load(const std::string& filename);

public:
	SpritePtr create(const std::string& filename);
};

#endif