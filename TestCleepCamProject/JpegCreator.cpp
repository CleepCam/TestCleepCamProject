#include "JpegCreator.h"

JpegCreator::JpegCreator()
{
}

JpegCreator :: ~JpegCreator()
{
	Cleanup();
}

void JpegCreator::takeScreenShot(roc_image frame) {
	Cleanup();
	FILE* outfile;
	fopen_s(&outfile, "test.jpeg", "w");
	jpeg_compress_struct cinfo;
	
	
	ErrorManager errorManager;

	if (!outfile) {
		cerr<< "can't open jpeg file\n";
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);

	// set our custom error handler
	cinfo.err =  jpeg_std_error(&errorManager.defaultErrorManager);
	errorManager.defaultErrorManager.error_exit = ErrorExit;
	errorManager.defaultErrorManager.output_message = OutputMessage;
	if (setjmp(errorManager.jumpBuffer))
	{
		// We jump here on errorz
		Cleanup();
		jpeg_destroy_compress(&cinfo);
		fclose(outfile);
		return;
	}
	jpeg_create_compress(&cinfo);
	
	cinfo.image_height = frame.height;
	cinfo.image_width = frame.width;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	JSAMPROW row_pointer;          /* pointer to a single row */

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer = (JSAMPROW) get_intensity(frame, 1, cinfo.image_width, cinfo.image_height);
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}
	jpeg_finish_compress(&cinfo);
	/* After finish_compress, we can close the output file. */
	fclose(outfile);
}

uint8_t* JpegCreator::get_intensity(roc_image image,
	size_t channel,
	size_t column,
	size_t row)
{
	const size_t channels = (image.color_space == ROC_BGR24 ? 3 : 1);
	const size_t index = row*image.step + column*channels + channel;
	return &image.data[index];
}


void JpegCreator::Cleanup()
{
	if (m_pImageInfo)
	{
		delete[] m_pImageInfo->data;
		delete m_pImageInfo;
		m_pImageInfo = NULL;
	}
}

void JpegCreator::ErrorExit(j_common_ptr cinfo)
{
	// cinfo->err is actually a pointer to my_error_mgr.defaultErrorManager, since pub
	// is the first element of my_error_mgr we can do a sneaky cast
	ErrorManager* pErrorManager = (ErrorManager*)cinfo->err;
	(*cinfo->err->output_message)(cinfo); // print error message (actually disabled below)
	longjmp(pErrorManager->jumpBuffer, 1);
}


void JpegCreator::OutputMessage(j_common_ptr cinfo)
{
	// disable error messages
	/*char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message) (cinfo, buffer);
	fprintf(stderr, "%s\n", buffer);*/
}