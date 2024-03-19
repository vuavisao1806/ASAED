#ifndef HEADER_ASAED_VIDEO_COMPOSITOR_HPP
#define HEADER_ASAED_VIDEO_COMPOSITOR_HPP

#include <vector>
#include <memory>

class DrawingContext;
class DrawingRequest;

class Compositor final {
private:
	/** to hold the memory of all drawing requests */
	std::vector<std::unique_ptr<DrawingRequest>> m_request_holder;

	/** to manage many different drawing context */
	std::vector<std::unique_ptr<DrawingContext>> m_drawing_contexts;

public:
	Compositor();
	~Compositor();

private:
	Compositor(const Compositor&) = delete;
	Compositor& operator=(const Compositor&) = delete;

public:
	DrawingContext& make_context();
	void render();
};

#endif