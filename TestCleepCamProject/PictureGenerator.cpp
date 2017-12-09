#include "PictureGenerator.h"



PictureGenerator::PictureGenerator()
{
}


PictureGenerator::~PictureGenerator()
{
	clearMat();
}

void PictureGenerator::kindredFrameCapture(roc_image &frame, string& outputDir, int &index)
{
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	Image.create(frame.height, frame.width, CV_8UC3);
	Image.data = frame.data;
	String userPic = outputDir + "\\" +  "frame.png";
	if (!Image.empty())
		imwrite(userPic, Image, compression_params);

	else cerr << "\nError: Image is null\n";
	//clearMat();
}

void PictureGenerator::captureKindredFace(roc_template & face, string &usrDirectory, int &index, char* filename)
{
	roc_image image = extrapolateFace(face);
	string fullPath = usrDirectory + filename + to_string(index) + ".png";
	roc_write_image(image, fullPath.c_str());
	roc_free_image(image);
}

void PictureGenerator::captureStrangerFace(roc_template & face, string &usrDirectory, int &index)
{
	roc_image image = extrapolateFace(face);
	string fullPath = usrDirectory + "\\s_" + to_string(index) + ".png";
	roc_write_image(image, fullPath.c_str());
	roc_free_image(image);
}

void PictureGenerator::capturePossibilityFace(roc_template & face, string &usrDirectory, int &index)
{
	roc_image image = extrapolateFace(face);
	string fullPath = usrDirectory + "\\possibly_" + to_string(index) + ".png";
	roc_write_image(image, fullPath.c_str());
	roc_free_image(image);
}

roc_image PictureGenerator::extrapolateFace(roc_template &face)
{
	roc_image image;
	error = roc_decode_image(face.tn_size, face.tn, ROC_BGR24, &image);
	if (error) {
		cerr << "Roc Error Decoding template: " << error << endl;
	}
	return image;
}

void PictureGenerator::clearMat()
{
	//delete[] Image.data;
	Image.release();
}


