#include "object/portal.hpp"

Portal::Portal(const Vector& position) :
	MovingSprite(Vector(0.0f, 0.0f), "data/images/object/portal_big/portal.json")
{
	set_pos(position - get_bounding_box().get_middle());
}

void Portal::update(float /* dt_sec */) {}
void Portal::draw(DrawingContext& drawing_context) {
	MovingSprite::draw(drawing_context);
}

bool Portal::is_singleton() const { return true; }