#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "VideoProcessor.h"

using namespace std;

int main(int argc, char *argv[])
{
	VideoProcessor vidP;
	size_t index = -1;
	float match = 0.0;

	if ((argc < 3) || (argc > 4)) {
		cerr << "Expected 2 or 3 arguments only: \n"
			"Index number and gallery file, and video file\n";
		return EXIT_FAILURE;
	}

	else if (argc == 4) {
	vidP.getMVPGalleries(argv[1], argv[2]);
	vidP.processVideo(argv[3], argc);
	}
	else if (argc == 3) {
		if (vidP.getMVPGalleries(argv[1], argv[2]))
		vidP.processVideo(argv[2], argc);
	}
	
	return EXIT_SUCCESS;
}
