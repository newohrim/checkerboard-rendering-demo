#include <iostream>
#include <fstream>

#pragma once
class frames_counter
{
private:
	const std::string file_name = "report.txt";
	const int FIRST_FRAMES_IGNORE_COUNT = 5;
	size_t total_frames_count = 0;
	double total_frametime = 0.0f;
	int measurements_count = 0;
	int frames_count = 0;
	int fps = 0;
	double timer = 0.0f;
	double period = 1.0f;
	int lock = 0;
	void reset();

public:
	void log_in_file();
	void set_period(const double update_period);
	void add_frame(const double frame_time);
	int get_fps();
};

