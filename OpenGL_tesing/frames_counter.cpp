#include "frames_counter.h"

void frames_counter::reset()
{
	timer = 0.0f;
	frames_count = 0;
}

void frames_counter::set_period(const double update_period)
{
	//if (update_period < 0.0f)
	//	throw new std::runtime_error("FPS update period can't be negative.");
	period = update_period;
}

void frames_counter::add_frame(const double frame_time)
{
	++frames_count;
	timer += frame_time;
	if (timer >= 1.0f) 
	{
		fps = frames_count;
		reset();
	}
}

int frames_counter::get_fps()
{
	return fps;
}
