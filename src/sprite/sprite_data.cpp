#include "sprite/sprite_data.hpp"


#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"
#include "video/surface.hpp"

SpriteData::Action::Action() :
	name(),
	family_name(),
	x_offset(0),
	y_offset(0),
	hitbox_w(0),
	hitbox_h(0),
	fps(8),
	surfaces()
{}

SpriteData::~SpriteData() {
	actions.clear();
}

SpriteData::SpriteData(const std::string& file_name) :
	actions(),
	name(file_name)
{
	ReaderMachine reader(file_name);
	
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse_action(reader.get_data(i));
	}

}

void SpriteData::parse_action(const ReaderData* data) {
	auto action = std::make_unique<Action>();

	if (!data->get("name", action->name)) {
		if (!actions.empty()) {
			throw std::runtime_error("If there are more than one action, they MUST have names!");
		}
	}

	data->get("family-name", action->family_name);

	std::vector<float> hitbox;
	if (data->get("hitbox", hitbox)) {
		switch (hitbox.size()) {
			case 4:
				action->x_offset = hitbox[0];
				action->y_offset = hitbox[1];
				action->hitbox_w = hitbox[2];
				action->hitbox_h = hitbox[3];
				break;
			
			default:
				throw std::runtime_error("hitbox must specify 4 coordinates");
				break;
			}
	}

	data->get("fps", action->fps);
	
	std::string mirror_action;
	if (data->get("mirror-action", mirror_action)) {
		const auto& mirror_temp = get_action(mirror_action);
		if (mirror_temp == nullptr) {
			std::ostringstream msg;
			msg << "Could not mirror action. Action not found: \"" << mirror_action << "\"\n";
			msg << "Mirror actions must be defined after the real one!";
			throw std::runtime_error(msg.str());
		}
		else {
			action->fps = mirror_temp->fps;
			
			action->x_offset = mirror_temp->x_offset;
			action->y_offset = mirror_temp->y_offset;
			action->hitbox_w = mirror_temp->hitbox_w;
			action->hitbox_h = mirror_temp->hitbox_h;
			
			for (const auto& surf : mirror_temp->surfaces) {
				auto surface = surf->clone_flip(HORIZONTAL_FLIP);
				action->surfaces.push_back(surface);
			}
		}
	}
	else {
		// load real images
		std::vector<std::string> images;
		if (data->get("images", images)) {
			for (const auto& filename : images) {
				auto surface = Surface::from_file(data->m_parent_path + filename);
				action->surfaces.push_back(std::move(surface));
			}
		}
		else {
			std::ostringstream msg;
			msg << "Sprite '" << name << "' does not contain images belong action '" << action->name << "'";
			throw std::runtime_error(msg.str());
		}
	}

	actions[action->name] = std::move(action);
}

const SpriteData::Action* SpriteData::get_action(const std::string& name) const {
	auto it = actions.find(name);
	if (it == actions.end()) {
		return nullptr;
	}
	return it->second.get();
}