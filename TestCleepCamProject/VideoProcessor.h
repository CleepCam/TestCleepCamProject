#pragma once
#include <iostream>
#include <algorithm>
#include "Timer.h"
#include "PictureGenerator.h"
#include "StringHandler.h"

#define MAX_FACES_TO_DETECT 10
using namespace std;
class VideoProcessor
{
private:
	vector<roc_template> retrievedTemplates;
	roc_similarity similarity;
	roc_time timeStamp;
	size_t *size;
	static int k;
	roc_gallery* mvpGalleries;
	Timer timer;
	int amountOfEnrolled;
	float SIMILARITY_THRESHOLD;
	PictureGenerator pic;
	StringHandler handler;
	vector<int> gallery_indices, configSettings;
	string usrDirectory;
	roc_error error;
	roc_video video;
	roc_image frame;

public:
	VideoProcessor();
	~VideoProcessor();

	bool getMVPGalleries(char*, char*);
	//roc_template getMVPTemplates( int);
	void processVideo(char*, int);
	void facialAnalysis(roc_template*, int);
	void createFrameImage(int&, char*);
	void createFaceImage(int &, roc_template&);
	bool loadGalleries( int noOfEnrolled, int index, bool previousSucess);
	bool compareImages(roc_template*);
	void searchForMatch(int sizeOfProbe, int index, bool forStrangers, int foundIndex, roc_gallery&, roc_gallery&);
	bool detectStranger(roc_template*);
	//bool getEnrolledGallery(char* galleryFile);
	void cleanUp(roc_template *aTemplate, roc_video &video, roc_image &frame);
};

