#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;


class StringHandler
{
private:
	stringstream ss;
	vector<int> indexes, configs;
	string directory;
	char* config;


public:
	StringHandler();
	~StringHandler();
	vector<int> convertStringIndex(char* );
	string getDirectory(char* filename);
	vector<int> getConfigSetting(string& );
};

