#include "video/color.hpp"

const Color Color::BLACK(0.0f, 0.0f, 0.0f);
const Color Color::RED(1.0f, 0.0f, 0.0f);
const Color Color::GREEN(0.0f, 1.0f, 0.0f);
const Color Color::BLUE(0.0f, 0.0f, 1.0f);
const Color Color::WHITE(1.0f, 1.0f, 1.0f);

Color::Color() :
	red(1.0f),
	green(1.0f),
	blue(1.0f),
	alpha(1.0f)
{}

Color Color::from_rgb(uint8_t red_, uint8_t green_, uint8_t blue_) {
	return Color(static_cast<float>(red_ / 255.0f),
	             static_cast<float>(green_ / 255.0f),
	             static_cast<float>(blue_ / 255.0f));
}

Color::Color(float red_, float green_, float blue_, float alpha_) :
	red(red_),
	green(green_),
	blue(blue_),
	alpha(alpha_)
{}

uint8_t Color::r8() const { return static_cast<uint8_t>(red * 255.0f); }
uint8_t Color::g8() const { return static_cast<uint8_t>(green * 255.0f); }
uint8_t Color::b8() const { return static_cast<uint8_t>(blue * 255.0f); }
uint8_t Color::a8() const { return static_cast<uint8_t>(alpha * 255.0f); }


Color Color::operator+(const Color& other) const { return Color(red + other.red, green + other.green, blue + other.blue, alpha + other.alpha); }
Color Color::operator-(const Color& other) const { return Color(red - other.red, green - other.green, blue - other.blue, alpha - other.alpha); }
Color Color::operator*(const Color& other) const { return Color(red * other.red, green * other.green, blue * other.blue, alpha * other.alpha); }
Color Color::operator/(const Color& other) const { return Color(red / other.red, green / other.green, blue / other.blue, alpha / other.alpha); }

Color Color::operator*(float m) const { return Color(red * m, green * m, blue * m, alpha * m); }
Color Color::operator/(float d) const { return Color(red / d, green / d, blue / d, alpha / d); }