#ifndef CONFIG_H
#define CONFIG_H

#define MIN_WINDOW_WIDTH 800
#define MAX_WINDOW_WIDTH 7680
#define MIN_WINDOW_HEIGHT 600
#define MAX_WINDOW_HEIGHT 4320
#define MIN_ROTATION_SPEED 1
#define MAX_ROTATION_SPEED 10000
#define MIN_CUBE_COUNT 1
#define MAX_CUBE_COUNT 43
#define MIN_INTERPOLATION_COUNT 1
#define MAX_INTERPOLATION_COUNT 9

#define DEFFAULT_WINDOW_WIDTH 1280
#define DEFFAULT_WINDOW_HEIGHT 720
#define DEFFAULT_ROTATION_SPEED 1
#define DEFFAULT_CUBE_COUNT 15
#define DEFFAULT_INTERPOLATION_COUNT 1

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class configuration
{
public:
	// Window's width global value.
	static int WINDOW_WIDTH;
	// Window's height global value.
	static int WINDOW_HEIGHT;
	// Near clipping plane dist global value.
	static float NEAR_CLIPPING_PLANE_DIST;
	// Far clipping plane dist global value.
	static float FAR_CLIPPING_PLANE_DIST;
	// Field of view global value.
	static float FOV;
	// Camera offset in Z axis from scene.
	static float CAMERA_OFFSET;

	// Initializes configuration object. Reads all values from file config.cfg.
	void init();
	// Returns config value by its full name.
	int operator[](const string& field) { return config_data[field]; }

private:
	// Path to configuration file.
	const char* path = "config.cfg";\
	// Map of all config parametres.
	map<string, int> config_data;
	// Read stream.
	ifstream in;
	// Splits string in parts by space symbol.
	vector<string> split(const string& sequence);
	// Validates all parametres values.
	void check_values();
};

#endif 
