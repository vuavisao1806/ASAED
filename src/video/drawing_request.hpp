#ifndef HEADER_ASAED_VIDEO_DRAWING_REQUEST_HPP
#define HEADER_ASAED_VIDEO_DRAWING_REQUEST_HPP

#include "math/rectf.hpp"
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
	Flip flip;

	DrawingRequest() = delete;
	// needed when use dynamic_cast
	DrawingRequest(RequestType type_) :
		type(type_),
		flip()
	{}

	virtual ~DrawingRequest() {};
};

struct TextureRequest : public DrawingRequest {
	const Texture* texture;
	// to batch draw
	std::vector<Rectf> srcrects;
	// to batch draw
	std::vector<Rectf> dstrects;

	TextureRequest() :
		DrawingRequest(TEXTURE),
		texture(),
		srcrects(),
		dstrects()
	{}

private :
	TextureRequest(const TextureRequest&) = delete;
	TextureRequest& operator=(const TextureRequest&) = delete;
};

struct FillRectRequest : public DrawingRequest {
	Rectf rect;
	
	FillRectRequest() :
		DrawingRequest(FILLED_RECT),
		rect()
	{}
};

#endif