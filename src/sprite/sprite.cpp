#include "sprite/sprite.hpp"

#include "asaed/globals.hpp"
#include "util/log.hpp"

Sprite::~Sprite()
{}

Sprite::Sprite(const Sprite& other) :
	m_data(other.m_data),
	m_frame(other.m_frame),
	m_frame_index(other.m_frame_index),
	m_last_ticks(other.m_last_ticks),
	m_angle(0.0f),
	m_alpha(1.0f),
	m_color(Color::WHITE),
	m_action(other.m_action)
{}

Sprite::Sprite(SpriteData& data) :
	m_data(data),
	m_frame(0),
	m_frame_index(0),
	m_last_ticks(g_game_time),
	m_angle(),
	m_alpha(1.0f),
	m_color(),
	m_action(m_data.actions.begin()->second.get())
{}

SpritePtr Sprite::clone() const {
	auto sprite = std::make_unique<Sprite>(*this);
	return sprite;
}

void Sprite::update() {
	float m_frame_inc = m_action->fps * (g_game_time - m_last_ticks);
	m_last_ticks = g_game_time;

	m_frame += m_frame_inc;

	m_frame_index += static_cast<int>(m_frame);
	m_frame -= static_cast<int>(m_frame);

	m_frame_index %= get_frames();
}

void Sprite::draw(Canvas& canvas, const Vector& pos, int layer, Flip flip) {
	assert(m_action != nullptr);
	update();

	DrawingContext& context = canvas.get_drawing_context();
	context.push_transform();

	context.set_flip(context.get_flip() ^ flip);
	context.set_alpha(context.get_alpha() * m_alpha);

	canvas.draw_surface(m_action->surfaces[m_frame_index],
	                    pos - Vector(get_current_offset_x(), get_current_offset_y()),
	                    m_angle, m_color, layer); 

	context.pop_transform();
}

void Sprite::draw_scaled(Canvas& canvas, const Rectf& dstrect, int layer, Flip flip) {
	assert(m_action != nullptr);
	update();

	DrawingContext& context = canvas.get_drawing_context();
	context.push_transform();

	context.set_flip(context.get_flip() ^ flip);
	context.set_alpha(context.get_alpha() * m_alpha);

	canvas.draw_surface_scaled(m_action->surfaces[m_frame_index],
	                           dstrect, m_color, layer);

	context.pop_transform();
}
	
void Sprite::set_action(const std::string& name) {
	if (m_action->name == name) return;
	
	const SpriteData::Action* new_action = m_data.get_action(name);
	if (!new_action) {
		log_debug << "Action '" << name << "' not found\n";
		return;
	}

	if (m_action->family_name != new_action->family_name) {
		m_frame_index = 0;
		m_frame = 0.0f;
	}
	m_action = new_action;
}

int Sprite::get_frames() { return static_cast<int>(m_action->surfaces.size()); }

int Sprite::get_current_frame() { return m_frame_index; }

float Sprite::get_current_frame_progress() { return m_frame; }

float Sprite::get_current_offset_x() { return m_action->x_offset; }
float Sprite::get_current_offset_y() { return m_action->y_offset; }
float Sprite::get_current_hitbox_width() { return m_action->hitbox_w; }
float Sprite::get_current_hitbox_height() { return m_action-> hitbox_h; }

void Sprite::set_angle(float angle) { m_angle = angle; }
float Sprite::get_angle() const { return m_angle; }
void Sprite::set_alpha(float alpha) { m_alpha = alpha; }
float Sprite::get_alpha() const { return m_alpha; }
void Sprite::set_color(Color color) { m_color = color; }
Color Sprite::get_color() const { return m_color; }