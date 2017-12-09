#include "VideoProcessor.h"


VideoProcessor::VideoProcessor()
{
	error = roc_initialize("ROC.lic", NULL);
	if (error) {
		clog << error;
		exit(EXIT_FAILURE);
	}
}
VideoProcessor::~VideoProcessor() {}

bool VideoProcessor::compareImages(roc_template* toCompare) {
	//roc_template savedTemplate = getMVPTemplate(index);
	bool isPastThreshold = false;
	vector<int>::iterator itr;
	roc_gallery temp;
	size_t pSize;
	int i = 0;
	k = 1;
	
	error = roc_open_gallery(NULL, &temp, NULL);

	if (error) {
		cerr << "Program is having trouble with ROC SDK";
		return false;
	}


	for (auto i = 0; i <= MAX_FACES_TO_DETECT; i++) {

		if (!toCompare[i].algorithm_id & ROC_INVALID) {
			error = roc_enroll(temp, toCompare[i]);
			roc_size(temp, &pSize);
			if (error)cerr << "Roc Error:  "<< error << endl;
		}
	}


	for (itr = gallery_indices.begin(); itr != gallery_indices.end(); itr++) {
		//roc_knn(mvpGalleries[*itr], temp, k, SIMILARITY_THRESHOLD, neighbors );
		searchForMatch(pSize, 0, false, *itr, temp, mvpGalleries[*itr]);
	}

	//int size_of_vector = retrievedTemplates.size();

	/*for (template_iterator = retrievedTemplates.begin(), i = 0;
		i <=arrSize; i++)
	{
		if ((i == arrSize) && template_iterator != retrievedTemplates.end()) {
			i = 0;
			++template_iterator;
			if (template_iterator == retrievedTemplates.end())break;
		}

		//if (toCompare[i] == NULL)break;

		roc_compare_templates(toCompare[i], (*template_iterator), &similarity);
		if (similarity >= SIMILARITY_THRESHOLD) {
			isPastThreshold = true;
			break;
		}
	}*/
	return isPastThreshold;
}

void VideoProcessor::searchForMatch(int sizeOfProbe, int index, bool forStrangers, int toSearchIndex,
	roc_gallery &eachMVPGall, roc_gallery &probes) {
	bool isStrangerDetected = false;
	if (index < sizeOfProbe) {
		int k = size[toSearchIndex];
		roc_candidate *neighbors = (roc_candidate*)malloc( k*
			sizeof(roc_candidate));

		roc_template tProbe;
		roc_at(probes, index, &tProbe);
		error = roc_search(eachMVPGall, tProbe, k, 0.0, neighbors);
		if (error) {
			cerr << "Roc Error occured when searching for simillarities: " << error << endl;
			roc_free_template(&tProbe);
			return;
		}
		if (neighbors[0].index != ROC_INVALID_TEMPLATE_INDEX) {
			if (!forStrangers) {
				if ((neighbors[0].similarity >= SIMILARITY_THRESHOLD)) {
					cout << "Match:" << toSearchIndex << ":time:" << timeStamp << endl;

					createFrameImage(toSearchIndex);
					createFaceImage(toSearchIndex, tProbe);
				}
				roc_free_template(&tProbe);
				searchForMatch(sizeOfProbe, ++index, forStrangers, toSearchIndex, eachMVPGall, probes);
			}else {
				if (neighbors[0].similarity < 0.36) {
					cout << "Poor similarity";

				}
				else if ((neighbors[0].similarity >= 0.36) &&
						(neighbors[0].similarity < SIMILARITY_THRESHOLD)) {
					cout << "Possibily:" << toSearchIndex << ":time:" << timeStamp << endl;
					createFaceImage(toSearchIndex, tProbe);
					
				}
			}
		}
	}	
}

bool VideoProcessor::detectStranger(roc_template *templates)
{
	//if (templates!=NULL && templates[0] != NULL) {
	bool strangerDetected = false;
		roc_gallery tempGallery;
		roc_similarity toAnyOne;
		size_t psSize;
		
		roc_ensure(roc_open_gallery(NULL, &tempGallery, NULL));
			
			for (auto i = 0; i < MAX_FACES_TO_DETECT; i++) {
				if (!(templates[i].algorithm_id & ROC_INVALID)) {
					error = roc_enroll(tempGallery, templates[i]);
					roc_size(tempGallery, &psSize);
					if (error) {
						cerr << "ROC Error in creating stranger gallery" << endl;
						return false;
					}

				}
			}
			roc_candidate *couldBe = (roc_candidate*)malloc(psSize *sizeof(roc_candidate));

			roc_ensure(roc_knn(mvpGalleries, tempGallery, psSize, 0.0, couldBe));

				if (couldBe[]->similarity<0.36 || couldBe[]->index<0) {
					createFrameImage(couldBe[].index);
					strangerDetected = true;
				}
		
		roc_close_gallery(tempGallery);
		return strangerDetected;
}

void VideoProcessor::cleanUp(roc_template *aTemplate, roc_video &video, roc_image &frame) {
	vector<roc_template>::iterator template_iterator;
	int arrSize = sizeof(aTemplate) / sizeof(aTemplate[0]);
	for (auto j = 0; j <MAX_FACES_TO_DETECT; j++) {
		if (!(aTemplate[j].algorithm_id & ROC_INVALID)) {
			roc_free_template(&aTemplate[j]);
		}
		else break;
	}
	for (template_iterator = retrievedTemplates.begin(); template_iterator != retrievedTemplates.end(); 
		++template_iterator) {
		roc_free_template(&*template_iterator);
	}
	roc_ensure(roc_free_image(frame));
	roc_ensure(roc_close_video(video));
}

void VideoProcessor::processVideo(char* videofile, int argc) {

	roc_color_space color = ROC_BGR24;
	roc_set_thumbnail_parameters(192, 256, 0.6, 32);

	roc_algorithm_id id = ROC_FRONTAL | ROC_PAD | ROC_ROLL | ROC_FR| ROC_LIPS
		| ROC_DEMOGRAPHICS | ROC_GLASSES | ROC_LANDMARKS | ROC_THUMBNAIL; 
	roc_template *vidTemplates = (roc_template*)malloc(MAX_FACES_TO_DETECT * sizeof(roc_template));

	size_t adaptive_minimum_size;
	error = roc_open_video(videofile, color, &video, NULL);
	if (error) {
		cerr << "ROC Error:" << error;
		roc_ensure(roc_finalize());
	}

	else {
		//ROC_ENSURE(roc_open_video(videofile, color, &video, NULL))
		do{
			error = roc_read_frame(video, &frame, &timeStamp);
			if (error) {
				clog << "ROC Error:" << error;
				roc_ensure(roc_close_video(video));
				roc_ensure(roc_finalize());
				break;
			}
			else {
				error =roc_adaptive_minimum_size(frame, 0.08f, 36, &adaptive_minimum_size);
				if (error) {
					clog << "ROC Error:" << error;
					roc_ensure(roc_free_image(frame));
					roc_ensure(roc_close_video(video));
					roc_ensure(roc_finalize());
					break;
				}
				else {
					error = roc_represent(frame, id, adaptive_minimum_size, MAX_FACES_TO_DETECT,
						0.02f, vidTemplates);
					if (error) {
						clog << "ROC Error:" << error;
						roc_ensure(roc_free_image(frame));
						roc_ensure(roc_close_video(video));
						roc_ensure(roc_finalize());
						break;
					}
					else {
						if (!(vidTemplates[0].algorithm_id & ROC_INVALID)) {
							facialAnalysis(vidTemplates, argc);
						}
						else {
							cout << "No Faces detected!\n";

							//cleanUp(vidTemplates, video, frame);
							//processVideo(videofile, argc);
						}
					}
				}
			}
		} while (video != NULL);
	}
}

void VideoProcessor::facialAnalysis(roc_template* temps, int argc){
	if (argc == 4){
		if (compareImages(temps)) {
			cout << "Match";
		}
		else {
			if (detectStranger(temps)) {
				cout << "Alert! Stranger Seen!!!\n";
			}
		}
	}
	else if (argc == 3) {
		if (detectStranger(temps)) {
			cout << "Alert! Stranger Seen!!!\n";
		}
	}
}

bool VideoProcessor::getMVPGalleries(char* csvIndexes, char * directory) {
	//int count = 0;
	bool upload_successful = false;
	usrDirectory = handler.getDirectory(directory);
	gallery_indices = handler.convertStringIndex(csvIndexes);
	configSettings = handler.getConfigSetting(usrDirectory);
	SIMILARITY_THRESHOLD = configSettings.at(0)/100;
	amountOfEnrolled = configSettings.at(1);
	mvpGalleries = (roc_gallery*)malloc( amountOfEnrolled* sizeof(roc_gallery));
	size = new size_t[configSettings.at(1)];
	//vector<int>::iterator it = gallery_indices.begin();
	
	upload_successful = loadGalleries(amountOfEnrolled, 0, false);
		/*while (it!=gallery_indices.end()) {
			retrievedTemplates.push_back(getMVPTemplates(*it));
			++it;
		}*/
	return upload_successful;
}
/*bool VideoProcessor::getEnrolledGallery(char* galleryDirectory) {
	usrDirectory=handler.getDirectory(galleryDirectory);

	//cout << str;
	//cout << user;
	roc_error error;
	error = roc_open_gallery(galleryDirectory, &mvpGalleries, NULL);
	if (error) {
		cerr << "ROC Error:" << error;
		roc_ensure(roc_finalize());
	}
	else {
		roc_ensure(roc_size(mvpGalleries, &size));
		if (size <= 0) {
			cerr << "No templates stored in gallery!" << endl;
			roc_ensure(roc_finalize());
		}
		else return true;
	}
	return false;
}

roc_template VideoProcessor::getMVPTemplates( int index)
{													
	roc_template tempRetrieved;
		roc_error error;
		if (size >index){
			error = roc_at(mvpGalleries, index, &tempRetrieved);
		if (error) {
			cerr << "ROC Error:" << error;
			roc_ensure(roc_finalize());
		}
			return (tempRetrieved);
	}
}*/

void VideoProcessor::createFrameImage(int& index, char* filename) {
	pic.FrameCapture(frame, usrDirectory, index);
}

bool VideoProcessor::loadGalleries( int noOfEnrolled, int index, bool previousSucess)
{
	bool upload_successful = previousSucess;
	if (index < (noOfEnrolled)) {
		string gallFile = usrDirectory + to_string(index) + ".txt";
		error = roc_open_gallery(gallFile.c_str(), &mvpGalleries[index], NULL);
		if (error) {
			cerr << "ROC Error in opening gallery" << error;
			roc_ensure(roc_finalize());
			upload_successful = false;
		}
		else {
			error = roc_size(mvpGalleries[index], &size[index]);
			if ((size[index] <= 0)||error) {
				cerr << "No templates stored in gallery!" << endl;
				roc_ensure(roc_finalize());
				upload_successful = false;
			}
			else {
				cout << "Loaded gallery of index: " << index << endl;
				upload_successful = true;
				index++;
			}
		}
	}
	if (upload_successful)loadGalleries( noOfEnrolled, index, upload_successful);

	return upload_successful;
}
