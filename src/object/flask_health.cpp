#include "object/flask_health.hpp"

#include "asaed/game_manager.hpp"
#include "asaed/game_session.hpp"
#include "asaed/level.hpp"
#include "asaed/resources.hpp"
#include "control/controller.hpp"
#include "control/input_manager.hpp"
#include "asaed/room.hpp"
#include "object/player.hpp"
#include "gui/colorscheme.hpp"

FlaskHealth::FlaskHealth(const Vector& position) :
	MovingSprite(Vector(0.0f, 0.0f), "data/images/object/chest/flask_health.json")
{
	set_pos(position - get_bounding_box().get_middle());
}

void FlaskHealth::update(float /* dt_sec */) {}
void FlaskHealth::draw(DrawingContext& drawing_context) {
	MovingSprite::draw(drawing_context);
}

HitResponse FlaskHealth::collision(CollisionObject& other, const CollisionHit& /* hit */) {
	if (dynamic_cast<Player*>(&other)) {
		if (InputManager::current()->get_controller(0).pressed(Control::ATTACK)) {
			remove_me();
		}
	}
	return ABORT_MOVE;
}

bool FlaskHealth::is_singleton() const { return true; }

std::string FlaskHealth::class_name() { return "flask-health"; }
std::string FlaskHealth::get_class_name() const { return class_name(); }