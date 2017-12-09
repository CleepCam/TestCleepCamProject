#pragma once
#ifndef JPEG_LOADER_H
#define JPEG_LOADER_H
#include<iostream>
#include <roc.h>
#include <fstream> 

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>

#ifdef __cplusplus
}
#endif
using namespace std;


class JpegCreator
{
public:
	JpegCreator();
	void takeScreenShot(roc_image);//Function prototype for creating a Jpeg
	~JpegCreator();

private:
	roc_image* m_pImageInfo;//struct with pixel array
	void Cleanup();

	struct ErrorManager
	{
		jpeg_error_mgr defaultErrorManager;
		jmp_buf jumpBuffer;
	};
	static void ErrorExit(j_common_ptr cinfo);
	static void OutputMessage(j_common_ptr cinfo);
	uint8_t* get_intensity(roc_image image,
		size_t channel,
		size_t column,
		size_t row);
};
#endif

