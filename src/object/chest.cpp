#include "object/chest.hpp"

Chest::Chest(const Vector& position) :
	MovingSprite(Vector(0.0f, 0.0f), "data/images/object/chest/chest.json")
{
	set_pos(position - get_bounding_box().get_middle());
}

void Chest::update(float /* dt_sec */) {}
void Chest::draw(DrawingContext& drawing_context) {
	MovingSprite::draw(drawing_context);
}

bool Chest::is_singleton() const { return true; }