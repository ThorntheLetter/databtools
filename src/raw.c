#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "raw.h"
#include "datab.h"

rawdata rawToMem(FILE* f){
	rawdata r;

	fseek(f, 0, SEEK_END);
	r.size = ftell(f);
	fseek(f, 0, SEEK_SET);
	r.pixels = (uint8_t*) malloc(r.size);
	fread(r.pixels, 1, r.size, f);
	return r;
}

int memToRaw(rawdata r, FILE* f){	/*i am really bad at names*/
	if(fwrite(r.pixels, 1, r.size, f) != r.size){
		return 1;
	}
	return 0;
}