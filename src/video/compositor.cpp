#include "video/compositor.hpp"

#include "video/canvas.hpp"
#include "video/drawing_request.hpp"
#include "video/drawing_context.hpp"
#include "video/painter.hpp"
#include "video/renderer.hpp"
#include "video/video_system.hpp"

Compositor::Compositor() :
	m_request_holder(),
	m_drawing_contexts()
{}

Compositor::~Compositor() {
	m_drawing_contexts.clear();
	m_request_holder.clear();
}

DrawingContext& Compositor::make_context() {
	m_drawing_contexts.push_back(std::make_unique<DrawingContext>(m_request_holder));
	return *m_drawing_contexts.back();
}

void Compositor::render() {
	auto& renderer = static_cast<Renderer&>(VideoSystem::current()->get_renderer());
	renderer.start_draw();

	auto& painter = static_cast<Painter&>(VideoSystem::current()->get_painter());

	for (auto& drawing_context : m_drawing_contexts) {
		painter.set_clip_rect(drawing_context->get_viewport());
		drawing_context->get_canvas().render();
		painter.clear_clip_rect();
	}

	renderer.end_draw();

	// clean up everything
	for (auto& drawing_context : m_drawing_contexts) {
		drawing_context->clear();
	}
	VideoSystem::current()->present();
}
