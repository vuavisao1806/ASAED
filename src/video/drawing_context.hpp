#ifndef HEADER_ASAED_VIDEO_DRAWING_CONTEXT_HPP
#define HEADER_ASAED_VIDEO_DRAWING_CONTEXT_HPP

#include <memory>
#include <vector>

#include "math/rect.hpp"
#include "math/rectf.hpp"
#include "video/canvas.hpp"
#include "video/drawing_transform.hpp"

struct DrawingRequest;

class DrawingContext final {
private:
	/**
	 * to hold the memory of all drawing requests 
	 * it's reference (&) and it's shared between everything (Canvas, DrawingContext)
	*/
	std::vector<std::unique_ptr<DrawingRequest>>& m_request_holder;

	Rect m_viewport;
	
	/** to hold all of the transforms in the stack */
	std::vector<DrawingTransform> m_transform_stack;

	Canvas m_canvas;

public:
	~DrawingContext();

public:
	DrawingContext(std::vector<std::unique_ptr<DrawingRequest>>& request_holder);

private:
	DrawingContext(const DrawingContext&) = delete;
	DrawingContext& operator=(const DrawingContext&) = delete;

public:
	void push_transform();
	void pop_transform();

	DrawingTransform& transform();
	const DrawingTransform& transform() const;

	const Vector& get_translation() const;
	void set_translation(const Vector& new_traslation);

	void set_scale(float new_scale);
	void scale(float scale);
	float get_scale() const;

	void set_flip(Flip flip);
	Flip get_flip() const;

	void set_alpha(float new_alpha);
	float get_alpha() const;

	void set_viewport(const Rect& new_viewport);
	const Rect& get_viewport() const;

	float get_width() const;
	float get_height() const;
	Vector get_size() const;
	Rectf get_rect() const;
	Rectf get_cliprect() const;
	
	Canvas& get_canvas();

	void clear();
};

#endif