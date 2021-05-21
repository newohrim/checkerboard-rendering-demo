#define FIRST_FRAMES_IGNORE_COUNT 5

#include <iostream>
#include <fstream>

#pragma once
class frames_counter
{
private:
	// Path to output file.
	const std::string file_name = "report.txt";
	// Total count of frames.
	size_t total_frames_count = 0;
	// Total frametime, counted only once when timer was reset.
	double total_frametime = 0.0f;
	// Count of resets/seconds.
	int measurements_count = 0;
	// Count of frames from last reset.
	int frames_count = 0;
	// Current calculated fps.
	int fps = 0;
	// Current timer from last reset.
	double timer = 0.0f;
	// FPS update/reset period.
	double period = 1.0f;
	// Count of first ignored frames.
	int lock = 0;
	// Resets all local timers and counters.
	void reset();

public:
	// Reports calculated info to output file.
	void log_in_file();
	// Sets fps update period.
	void set_period(const double update_period);
	// Adds a frame.
	void add_frame(const double frame_time);
	// Returns current calculated fps.
	int get_fps();
};

