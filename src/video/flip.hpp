#ifndef HEADER_ASAED_VIDEO_FLIP_HPP
#define HEADER_ASAED_VIDEO_FLIP_HPP

enum {
	NO_FLIP         =   0b000,
	VERTICAL_FLIP   =   0b010,
	HORIZONTAL_FLIP =   0b100
};

typedef unsigned int Flip;

#endif