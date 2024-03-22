#ifndef HEADER_ASAED_SPRITE_SPRITE_DATA_HPP
#define HEADER_ASAED_SPRITE_SPRITE_DATA_HPP

#include <map>
#include <string>
#include <vector>
#include <sstream>

#include "video/surface_ptr.hpp"

class ReaderMachine;
class ReaderData;

class SpriteData {
	friend class Sprite;

public:
	~SpriteData();

public:
	SpriteData(const std::string& file_name);

private:
	struct Action {
		Action();
		
		/** name action */
		std::string name;
		/** family name action */
		std::string family_name;

		/** position correction (to cut image) (maybe only (0.0f, 0.0f))*/
		float x_offset;
		float y_offset;
		
		/** hitbox width and height (maybe full image)*/
		float hitbox_w;
		float hitbox_h;

		/** frame per second */
		float fps;
		
		/** holds all images to make sprite */
		std::vector<SurfacePtr> surfaces;
	};

	using Actions = std::map<std::string, std::unique_ptr<Action>>;

	Actions actions;
	std::string name;

public:
	void parse_action(const ReaderData* data);
	const Action* get_action(const std::string& name) const;
};

#endif