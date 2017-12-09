#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <roc.h>
#include <string>

using namespace cv;
using namespace std;

class PictureGenerator
{
private:
	Mat Image;
	roc_error error;
public:
	PictureGenerator();
	~PictureGenerator();

	void FrameCapture(roc_image &frame, string&, int&, char*);
	void captureKindredFace(roc_template &face, string&, int&, char *);
	void captureStrangerFace(roc_template &face, string&, int&, char *);
	void capturePossibilityFace(roc_template &face, string&, int&, char *);
	roc_image extrapolateFace(roc_template&);
	void clearMat();
};

