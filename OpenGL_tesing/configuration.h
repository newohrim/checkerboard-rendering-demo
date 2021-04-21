#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class configuration
{
public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static float NEAR_CLIPPING_PLANE_DIST;
	static float FAR_CLIPPING_PLANE_DIST;
	static float FOV;
	static float CAMERA_OFFSET;

	const char* path = "config.cfg";
	map<string, int> config_data;

	void init();
	int operator[](const string& field) { return config_data[field]; }

private:
	ifstream in;

	vector<string> split(const string& sequence);
};

#endif 
