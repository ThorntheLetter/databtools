#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#include "datab.h"

/*the bmp header, 54 bytes total*/
struct __attribute__((__packed__)) bmpHeader {
	/*file header*/
	uint16_t signature;	/* Needs to be 0x4D42 */
	uint32_t filesize; /* in bytes*/
	uint32_t reserved; /* needs to be 0, this is technically 2 16 bit 0s */
	uint32_t imageoffset; /* offset to image data */
	/*image header*/
	uint32_t headersize; /* size of the image header or whatever this part is called, is 40 */
	int32_t  width;
	int32_t  height;
	uint16_t planes; /* needs to be 1 */
	uint16_t bitsperpixel;
	uint32_t compression;
	uint32_t imagesize;
	int32_t  xres;
	int32_t  yres;
	uint32_t ncolours;
	uint32_t icolours;
};
typedef struct bmpHeader bmpHeader;

/*outputs the pixels in r to the file f as a bmp, returns 0 for no error, 1 for error*/
int memToBmp(rawdata r, FILE* f);

/*reads file f to rawdata struct and returns them*/
rawdata bmpToMem(FILE* f);

#endif