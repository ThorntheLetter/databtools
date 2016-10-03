#ifndef RAW_H
#define RAW_H

#include "datab.h"

/*outputs the pixels in r to the file f*/
int memToRaw(rawdata r, FILE* f);

/*reads file f to rawdata struct and returns them*/
rawdata rawToMem(FILE* f);

#endif