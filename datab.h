#ifndef DATAB_H
#define DATAB_H

#include <stdint.h>

struct dimensions {		/*for reference, bmp is signed 32 bit, jpeg is unsigned 16 bit, png is unsigned 32 bit*/
	uint32_t width;
	uint32_t height;
};
typedef struct dimensions dimensions;

struct rawdata {
	uint8_t* pixels;
	uint32_t size;
	dimensions dim;
};
typedef struct rawdata rawdata;

#endif