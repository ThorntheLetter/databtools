#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "raw.h"
#include "datab.h"

int rawToRaw(rawdata r, FILE* f){	/*i am really bad at names*/
	if(fwrite(r.pixels, 1, r.size, f) != r.size){
		return 1;
	}
	return 0;
}