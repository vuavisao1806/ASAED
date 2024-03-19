#ifndef HEADER_ASAED_VIDEO_CANVAS_HPP
#define HEADER_ASAED_VIDEO_CANVAS_HPP

#include <memory>
#include <vector>

#include "math/vector.hpp"
#include "math/rect.hpp"
#include "math/rectf.hpp"
#include "video/surface_ptr.hpp"
#include "video/color.hpp"
#include "video/renderer.hpp"

class DrawingContext;
struct DrawingRequest;

/**
 * Class to direct give DrawingRequest to Painter
 * (final processing step before the drawing)
*/

class Canvas final {
private:
	DrawingContext& m_drawing_context;
	
	/**
	 * to hold the memory of all drawing requests 
	 * it's reference (&) and it's shared between everything (Canvas, DrawingContext)
	*/
	std::vector<std::unique_ptr<DrawingRequest>>& m_request_holder;

	/** 
	 * to direct handle DrawingRequest
	 * the reason need m_requests placed in render()
	*/
	std::vector<DrawingRequest*> m_requests;

public:
	~Canvas();

private:
	Canvas(const Canvas&) = delete;
	Canvas& operator=(const Canvas&) = delete;

public:
	Canvas(DrawingContext& drawing_context, std::vector<std::unique_ptr<DrawingRequest>>& request_holder);

public:
	void render();

	void draw_surface(const SurfacePtr& surface, const Vector& position, int layer);
	void draw_surface(const SurfacePtr& surface, const Vector& position, float angle, const Color& color, int layer);

	void draw_surface_scaled(const SurfacePtr& surface, const Rectf& dstrect, const Color& color, int layer);
	void draw_surface_part(const SurfacePtr& surface, const Rectf& srcrect, const Rectf& dstrect, const Color& color, int layer);

	void draw_filled_rect(const Rectf& rect, const Color& color, int layer);
	
	void clear();

	DrawingContext& get_drawing_context();

private:
	Vector apply_translate(const Vector& pos) const;
	float scale() const;
};

#endif