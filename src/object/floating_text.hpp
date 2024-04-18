#ifndef HEADER_ASAED_OBJECT_FLOATING_TEXT_HPP
#define HEADER_ASAED_OBJECT_FLOATING_TEXT_HPP

#include "object/game_object.hpp"

#include <string>

#include "math/vector.hpp"
#include "util/timer.hpp"

class FloatingText final : public GameObject {
private:
	Vector m_position;
	std::string m_text;
	Timer m_timer;

public:
	FloatingText(const Vector& position, const std::string& text);
	FloatingText(const Vector& position, int number); // use for hurt

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
};

#endif