#pragma once

#include "graphics.h"

class Animation
{
public:
	Animation(TextureType m_start_texture, int m_time_per_frame, int m_number_of_frames);
	~Animation();

	void on_tick(unsigned int delta_time);
	void reset();

	TextureType get_texture_type();

private:
	TextureType m_start_texture;
	TextureType m_texture;

	int m_time_on_this_frame;

	int m_time_per_frame;

	int m_number_of_frames;
};

