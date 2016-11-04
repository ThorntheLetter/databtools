#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "datab.h"
#include "bmp.h"
#include "raw.h"

void help(){
	printf("Datab, a tool for preparing images for simple databending.\n"
		   "\n"
		   "Usage: datab [flags] [input filename]\n"
		   "\n"
		   "Input File Type Options\n"
		   "You must have exactly one of these.\n"
		   "\n"
		   "\t-r\t\theaderless interleaved bitmap\n"
		   "\t-b\t\tbmp\n"
		   "\n"
		   "Output File Type Options\n"
		   "You must have exactly one of these.\n"
		   "\n"
		   "\t-R\t\theaderless interleaved bitmap\n"
		   "\t-B\t\tbmp\n"
		   "\n"
		   "Other Options\n"
		   "\n"
		   "\t-x\t\tspecifies width of output file, requires non-headerless output file type\n"
		   "\t-y\t\tspecifies height of output file, requires non-headerless output file type\n"
		   "\t-o\t\tspecifies output file name, defaults to out\n"
		   "\t-h\t\tshows this message\n"
		   "\n");
}

int main(int argc, char** argv){
	char informat, outformat = 0;
	char* infilename;
	char* outfilename = "out";
	int c;
	dimensions size = {0};

	/*argument parsing*/
	if(argc==1){
		help();
		return 0;
	}
	while((c = getopt(argc, argv, "rbRBx:y:o:h")) != -1){
		switch(c){
			case 'r':
				if(informat != 0){
					fprintf(stderr, "error: multiple input formats specified\n\n");
					return 1;
				}
				informat = 'r';		/*r for raw*/
				break;
			case 'b':
				if(informat != 0){
					fprintf(stderr, "error: multiple input formats specified\n\n");
					return 1;
				}
				informat = 'b';		/*b for bmp*/
				break;
			case 'R':
				if(outformat != 0){
					fprintf(stderr, "error: multiple output formats specified\n\n");
					return 1;
				}
				outformat = 'R';		/*R for raw*/
				break;
			case 'B':
				if(outformat != 0){
					fprintf(stderr, "error: multiple output formats specified\n\n");
					return 1;
				}
				outformat = 'B';		/*B for bmp*/
				break;
			case 'x':
				if(size.width != 0){
					fprintf(stderr, "error: multiple widths specified\n\n");
					return 1;
				}
				size.width = atoi(optarg);
				break;
			case 'y':
				if(size.height != 0){
					fprintf(stderr, "error: multiple heights specified\n\n");
					return 1;
				}
				size.height = atoi(optarg);
				break;
			case 'o':
				outfilename = optarg;
				break;
			case 'h':
				help();
				return 0;
		}
	}
	if(informat == 0){
		fprintf(stderr, "error: no input format specified\n\n");
		return 1;
	}
	if(outformat == 0){
		fprintf(stderr, "error: no output format specified\n\n");
		return 1;
	}
	if(optind + 1 != argc){
		fprintf(stderr, "error: incorrect number of arguments\n\n");
		return 1;
	}
	infilename = argv[optind];

	/*input file to raw pixel pointer and dimensions*/
	FILE* infile = fopen(infilename, "rb");
	if(!infile){
		fprintf(stderr, "error: can not open input file\n\n");
		return 2;
	}
	rawdata data;
	switch(informat){
		case 'r':
			data = rawToMem(infile);
			break;

		case 'b':
			data = bmpToMem(infile);
			break;
	}
	if(!data.pixels){
		return 3;
	}
	fclose(infile);

	if(outformat != 'R' && (size.width || size.height)){
		if((size.height && size.width && size.height * size.width != data.size / 3) || (size.height && ((data.size % size.height) != 0)) || (size.width && ((data.size % size.width) != 0))){
			fprintf(stderr, "error: invalid dimensions, there are %d pixels\n\n", data.size / 3);
			return 1;	
		}
		*(size.height ? &data.dim.height : &data.dim.width) = size.height ? : size.width; 
		*(size.height ? &data.dim.width : &data.dim.height) = size.height ? data.size / (3 * size.height) : data.size / (3 * size.width);
	}

	FILE* outfile = fopen(outfilename, "wb");
	if(!outfile){
		return 2;
	}
	switch(outformat){
		case 'R':
			memToRaw(data, outfile);
			break;
		case 'B':
			memToBmp(data, outfile);
			break;
	}

	return 0;
}