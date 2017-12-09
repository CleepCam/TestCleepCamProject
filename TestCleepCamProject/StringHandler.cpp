#include "StringHandler.h"



StringHandler::StringHandler()
{
}


StringHandler::~StringHandler()
{
}

vector<int> StringHandler::convertStringIndex(char* indexCVS)
{
	ss.str(indexCVS);
	int index;
	//char* tok;
	while (ss >> index)
	{
		indexes.push_back(index);
		if (ss.peek() == ',')
			ss.ignore();
	}
	/*while (getline(ss, tok, ',')) {
		indexes.push_back(atoi(tok));
	}*/
	return indexes;
}

string StringHandler::getDirectory(char * filename)
{
	string strl(filename);
	directory = strl;

	return directory;
}

vector<int> StringHandler::getConfigSetting(string& directory)
{
	char data[100];
	ifstream infile;
	int  config;
	infile.open(directory + "\\Config.txt");

	cout << "Reading from the file" << endl;
	infile >> data;

	// write the data at the screen.
	cout << data << endl;
	ss.str(data);

	while (ss >> config)
	{
		configs.push_back(config);
		if (ss.peek() == ',')
			ss.ignore();
	}

	infile.close();
	return configs;
}
