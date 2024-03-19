#include "video/canvas.hpp"

#include <algorithm>

#include "video/color.hpp"
#include "video/surface_ptr.hpp"
#include "video/video_system.hpp"
#include "video/painter.hpp"
#include "video/surface.hpp"
#include "video/drawing_request.hpp"
#include "video/drawing_context.hpp"

Canvas::~Canvas()
{}

Canvas::Canvas(DrawingContext& drawing_context, std::vector<std::unique_ptr<DrawingRequest>>& request_holder) :
	m_drawing_context(drawing_context),
	m_request_holder(request_holder),
	m_requests()
{
	m_requests.reserve(180); // optional
}

void Canvas::render() {
	std::stable_sort(
		m_requests.begin(),
		m_requests.end(),
		[] (const DrawingRequest* lhs, const DrawingRequest* rhs) {
			return lhs->layer < rhs->layer;
		}
	);

	Painter& painter = VideoSystem::current()->get_painter();

	for (const auto& it : m_requests) {
		const DrawingRequest& request = *it;

		// to renderer inside viewport
		painter.set_clip_rect(request.viewport);

		switch (request.type) {
			case TEXTURE:
				painter.draw_texture(static_cast<const TextureRequest&>(request));
				break;

			case FILLED_RECT:
				painter.draw_filled_rect(static_cast<const FillRectRequest&>(request));
				break;
		
		default:
			break;
		}
	}
}

void Canvas::draw_surface(const SurfacePtr& surface, const Vector& position, int layer) {
	draw_surface(surface, position, 0.0f, Color(1.0f, 1.0f, 1.0f), layer);
}

void Canvas::draw_surface(const SurfacePtr& surface, const Vector& position, float angle, const Color& color, int layer) {
	if (!surface) return;

	const auto& cliprect = m_drawing_context.get_cliprect();
	if (position.x > cliprect.get_right() || position.y > cliprect.get_bottom()) return;
	if (position.x + static_cast<float>(surface->get_width()) < cliprect.get_left() ||
	    position.y + static_cast<float>(surface->get_height()) < cliprect.get_top()) return;

	m_request_holder.push_back(static_cast<std::unique_ptr<DrawingRequest>>(std::make_unique<TextureRequest>()));
	
	TextureRequest* request = static_cast<TextureRequest*>(m_request_holder.back().get());

	request->type = TEXTURE;
	request->layer = layer;
	request->flip = m_drawing_context.get_flip() ^ surface->get_flip();
	request->alpha = m_drawing_context.get_alpha();
	request->viewport = m_drawing_context.get_viewport();
	
	request->texture = surface->get_texture().get();
	request->srcrects.push_back(Rectf(surface->get_region()));
	request->dstrects.push_back(Rectf(apply_translate(position) * scale(), 
	                            Sizef(static_cast<float>(surface->get_width()) * scale(),
	                                  static_cast<float>(surface->get_height()) * scale())));
	request->angles.push_back(angle);
	request->color = color;

	m_requests.push_back(static_cast<DrawingRequest*>(request));
}


void Canvas::draw_surface_scaled(const SurfacePtr& surface, const Rectf& dstrect, const Color& color, int layer) {
	draw_surface_part(surface,
	                  Rectf(0.0f, 0.0f, 
	                        static_cast<float>(surface->get_width()), static_cast<float>(surface->get_height())),
	                  dstrect,
	                  color,
	                  layer);
}

void Canvas::draw_surface_part(const SurfacePtr& surface, const Rectf& srcrect, const Rectf& dstrect, const Color& color, int layer) {
	if (!surface) return;

	m_request_holder.push_back(static_cast<std::unique_ptr<DrawingRequest>>(std::make_unique<TextureRequest>()));
	
	TextureRequest* request = static_cast<TextureRequest*>(m_request_holder.back().get());

	request->type = TEXTURE;
	request->layer = layer;
	request->flip = m_drawing_context.get_flip() ^ surface->get_flip();
	request->alpha = m_drawing_context.get_alpha();
	request->viewport = m_drawing_context.get_viewport();
	
	request->texture = surface->get_texture().get();
	request->srcrects.push_back(srcrect);
	request->dstrects.push_back(Rectf(apply_translate(dstrect.p1()) * scale(), dstrect.get_size() * scale()));
	request->angles.push_back(0.0f);
	request->color = color;

	m_requests.push_back(static_cast<DrawingRequest*>(request));
}


void Canvas::draw_filled_rect(const Rectf& rect, const Color& color, int layer) {
	m_request_holder.push_back(static_cast<std::unique_ptr<DrawingRequest>>(std::make_unique<FillRectRequest>()));
	
	FillRectRequest* request = static_cast<FillRectRequest*>(m_request_holder.back().get());

	request->type = FILLED_RECT;
	request->layer = layer;

	request->flip = m_drawing_context.get_flip();
	request->alpha = m_drawing_context.get_alpha();
	request->viewport = m_drawing_context.get_viewport();

	request->rect = Rectf(apply_translate(rect.p1()) * scale(), rect.get_size() * scale());
	request->color = color;

	m_requests.push_back(static_cast<DrawingRequest*>(request));
}


void Canvas::clear() {
	m_requests.clear();
}

DrawingContext& Canvas::get_drawing_context() { return m_drawing_context; }

Vector Canvas::apply_translate(const Vector& pos) const {
	Vector translation = m_drawing_context.get_translation();
	return (pos - translation) + Vector(static_cast<float>(m_drawing_context.get_viewport().get_left()),
	                                    static_cast<float>(m_drawing_context.get_viewport().get_top()));
}

float Canvas::scale() const {
	return m_drawing_context.get_scale();
}

