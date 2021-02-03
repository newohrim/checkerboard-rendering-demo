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
	const char* path = "config.cfg";
	map<string, int> config_data;

	void init();
	int operator[](const string& field) { return config_data[field]; }

private:
	ifstream in;

	vector<string> split(const string& sequence);
};

#endif // !1
