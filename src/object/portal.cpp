#include "object/portal.hpp"

#include "asaed/resources.hpp"
#include "asaed/room.hpp"
#include "object/player.hpp"
#include "gui/colorscheme.hpp"

Portal::Portal(const Vector& position) :
	MovingSprite(Vector(0.0f, 0.0f), "data/images/object/portal_big/portal.json")
{
	set_pos(position - get_bounding_box().get_middle());
}

void Portal::update(float /* dt_sec */) {}
void Portal::draw(DrawingContext& drawing_context) {
	MovingSprite::draw(drawing_context);

	if (Room::get().get_bounding_box().contains(get_bounding_box())) {
		// draw hud
		if (get_bounding_box().contains(Room::get().get_nearest_player(get_pos())->get_bounding_box())) {
			drawing_context.get_canvas().draw_text(Resources::small_font, get_class_name(),
			                                       Vector(get_bounding_box().get_middle().x, get_bounding_box().p1().y),
			                                       FontAlignment::ALIGN_CENTER, LAYER_HUD, ColorScheme::Text::small_color);
		}
	}
}


HitResponse Portal::collision(CollisionObject& other, const CollisionHit& /* hit */) {
	return ABORT_MOVE;
}

bool Portal::is_singleton() const { return true; }

std::string Portal::class_name() { return "portal"; }
std::string Portal::get_class_name() const { return class_name(); }