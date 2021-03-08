#pragma once
class frames_counter
{
private:
	int frames_count = 0;
	int fps = 0;
	double timer = 0.0f;
	double period = 0.0f;
	void reset();

public:
	void set_period(const double update_period);
	void add_frame(const double frame_time);
	int get_fps();
};

