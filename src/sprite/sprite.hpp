#ifndef HEADER_ASAED_SPRITE_SPRITE_HPP
#define HEADER_ASAED_SPRITE_SPRITE_HPP

#include "sprite/sprite_ptr.hpp"
#include "sprite/sprite_data.hpp"
#include "video/canvas.hpp"
#include "video/drawing_context.hpp"

class Sprite final {
private:
	SpriteData& m_data;

	// between 0 and 1
	float m_frame;
	// between 0 and get_frames()
	int m_frame_index;
	float m_last_ticks;
	float m_angle;
	float m_alpha;
	Color m_color;
	
	const SpriteData::Action* m_action;

public:
	~Sprite();

public:
	Sprite(const Sprite& other);

public:
	Sprite(SpriteData& data);

public:
	SpritePtr clone() const;

public:
	/** update and draw the usual */
	void draw(Canvas& canvas, const Vector& pos, int layer, Flip flip = NO_FLIP);
	/** update and draw scale */
	void draw_scaled(Canvas& canvas, const Rectf& dstrect, int layer, Flip flip = NO_FLIP);
	
	/** set action by name */
	void set_action(const std::string& name);

	/** get the total frames in the current action */
	int get_frames();

	/** get the current active frame */
	int get_current_frame();

	float get_current_frame_progress();

	float get_current_offset_x();
	float get_current_offset_y();
	float get_current_hitbox_width();
	float get_current_hitbox_height();

	void set_angle(float angle);
	float get_angle() const;
	void set_alpha(float alpha);
	float get_alpha() const;
	void set_color(Color color);
	Color get_color() const;

private:
	void update();
};

#endif