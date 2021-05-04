#include "frames_counter.h"

void frames_counter::reset()
{
	timer = 0.0f;
	frames_count = 0;
}

void frames_counter::log_in_file()
{
	std::ofstream out(file_name);
	if (!out.is_open())
		std::cout << "CONFIG::ERROR : Unable to open report file." << std::endl;
	float average_fps = total_frames_count / (float)measurements_count;
	float average_frametime = (float)(total_frametime / measurements_count);
	out << "average fps = " << average_fps << std::endl;
	out << "average frametime = " << average_frametime << std::endl;
	out.close();
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
	if (timer >= period) 
	{
		fps = frames_count;
		total_frames_count += frames_count;
		total_frametime += frame_time;
		++measurements_count;
		reset();
	}
}

int frames_counter::get_fps()
{
	return fps;
}
