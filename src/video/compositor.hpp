#ifndef HEADER_ASAED_VIDEO_COMPOSITOR_HPP
#define HEADER_ASAED_VIDEO_COMPOSITOR_HPP

#include <vector>
#include <memory>

class DrawingContext;
class DrawingRequest;

/**
 * What is Compositor?
 * Compositor is the boss, it holds everything relevant to drawing requests and passes it to lower-grade
 * Moreover, it contains different drawing context (convenient to process)
 * What is purpose?
 * Compositor manages the memory of all requests and it's a very good
 * It avoid leak mem or sometimes we forget to remove something after called
 * After process, the memory of the drawing requests that needed to be removed is definitely removed (by only one line)
*/

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