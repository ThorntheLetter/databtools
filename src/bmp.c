#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "datab.h"

/* bmp needs to check colour space, dimensions since negative y and figure out where to start reading,
 transform bmp (bgr) to rgb colours, possibly through colour index*/

rawdata bmpToMem(FILE* f){
	uint32_t pixeloffset;
	uint16_t bitsperpixel;
	int32_t height;
	int reversed = 0;
	int padding = 0;
	rawdata r;

	fseek(f, 10, SEEK_SET);		/*offset in bytes to the pixel data*/
	fread(&pixeloffset, 4, 1, f);

	fseek(f, 18, SEEK_SET);		/*image width and height in pixels*/
	fread(&r.dim.width, 4, 1, f);
	fread(&height, 4, 1, f);
	if(height < 0){
		r.dim.height = abs(height);
		reversed = 1;
	} else {
		r.dim.height = height;
	}
	r.size = r.dim.height * r.dim.width * 3;
	r.pixels = (uint8_t*) malloc(r.size);

	fseek(f, 28, SEEK_SET);		/*bits per pixel*/
	fread(&bitsperpixel, 2, 1, f);

	switch(bitsperpixel){
		case 1:
			fprintf(stderr, "error: 1 bit per pixel bmp not yet supported\n\n");
			break;
		case 4:
			fprintf(stderr, "error: 4 bit per pixel bmp not yet supported\n\n");
			break;
		case 8:
			fprintf(stderr, "error: 8 bit per pixel bmp not yet supported\n\n");
			break;
		case 16:
			fprintf(stderr, "error: 16 bit per pixel bmp not yet supported\n\n");
			break;
		case 24:
			padding = 4 - (r.dim.width * 3) % 4;
			if(reversed){
				fseek(f, pixeloffset, SEEK_SET);
				int offset = 0;
				while(offset < r.size){
					fread(r.pixels + offset, 1, r.dim.width * 3, f);
					fseek(f, padding, SEEK_CUR);
					offset += r.dim.width * 3;
				}
			} else {
				int fullline = (r.dim.width * 3) + padding;
				int fileoffset = (fullline * (r.dim.height - 1)) + pixeloffset;
				int ptroffset = 0;
				fseek(f, fileoffset, SEEK_SET);
				while(ptroffset < r.size){
					fread(r.pixels + ptroffset, 1, r.dim.width * 3, f);
					ptroffset += (r.dim.width * 3);
					fileoffset -= (r.dim.width * 3) + padding;
					fseek(f, fileoffset, SEEK_SET);
				}

			}
			for(int i = 0; i < r.size; i += 3){
					uint8_t temp = *(r.pixels + i + 2);
					*(r.pixels + i + 2) = *(r.pixels + i);
					*(r.pixels + i) = temp;
			}
			break;
		case 32:
			fprintf(stderr, "error: 32 bit per pixel bmp not yet supported\n\n");
			break;
	}
	fseek(f, 0, SEEK_SET);
	return r;
}