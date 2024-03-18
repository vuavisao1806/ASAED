#ifndef HEADER_ASAED_VIDEO_DRAWING_REQUEST_HPP
#define HEADER_ASAED_VIDEO_DRAWING_REQUEST_HPP

#include "math/rectf.hpp"
#include "video/color.hpp"
#include "video/flip.hpp"
#include "video/texture.hpp"

enum RequestType {
	TEXTURE = 0,
	FILLED_RECT
};

// we can cast between DrawingRequest* and XYZRequest*
// but cast by the * (!!!), we can't cast values without losing data and maybe expensive (copy data)
struct DrawingRequest {
	RequestType type;

	int layer;
	Flip flip;
	float alpha;
	Rect viewport;

	DrawingRequest() = delete;
	DrawingRequest(RequestType type_) :
		type(type_),
		layer(),
		flip(),
		alpha(),
		viewport()
	{}

	// needed when use dynamic_cast
	virtual ~DrawingRequest() {};
};

struct TextureRequest : public DrawingRequest {
public:
	const Texture* texture;
	// to batch draw
	std::vector<Rectf> srcrects;
	// to batch draw
	std::vector<Rectf> dstrects;
	std::vector<float> angles;
	Color color;

public:
	TextureRequest() :
		DrawingRequest(TEXTURE),
		texture(),
		srcrects(),
		dstrects(),
		color()
	{}

private :
	TextureRequest(const TextureRequest&) = delete;
	TextureRequest& operator=(const TextureRequest&) = delete;
};

struct FillRectRequest : public DrawingRequest {
	Rectf rect;
	Color color;

	FillRectRequest() :
		DrawingRequest(FILLED_RECT),
		rect(),
		color()
	{}
};

#endif