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

	void init();
	int operator[](const string& field) { return config_data[field]; }

private:
	const char* path = "config.cfg";
	const int MIN_WINDOW_WIDTH = 600;
	const int MAX_WINDOW_WIDTH = 7680;
	const int MIN_WINDOW_HEIGHT = 600;
	const int MAX_WINDOW_HEIGHT = 4320;
	const int MIN_ROTATION_SPEED = 0;
	const int MAX_ROTATION_SPEED = 10000;
	const int MIN_CUBE_COUNT = 1;
	const int MAX_CUBE_COUNT = 43;

	const int DEFFAULT_WINDOW_WIDTH = 1280;
	const int DEFFAULT_WINDOW_HEIGHT = 720;
	const int DEFFAULT_ROTATION_SPEED = 1;
	const int DEFFAULT_CUBE_COUNT = 15;

	map<string, int> config_data;
	ifstream in;
	vector<string> split(const string& sequence);
	void check_values();
};

#endif 
