#include "animation.h"


Animation::Animation(TextureType start_texture, int time_per_frame, int number_of_frames)
{
	m_start_texture = start_texture;
	m_texture = start_texture;

	m_time_on_this_frame = 0;

	m_time_per_frame = time_per_frame;

	m_number_of_frames = number_of_frames;
}

Animation::~Animation()
{

}

void Animation::on_tick(unsigned int delta_time)
{
	m_time_on_this_frame += delta_time;

	if (m_time_on_this_frame >= m_time_per_frame) 
	{
		m_time_on_this_frame = m_time_on_this_frame - m_time_per_frame;
		m_texture = (TextureType)(m_texture + 1);

		if (m_texture - m_start_texture >= m_number_of_frames)
		{
			m_texture = m_start_texture;
		}
	}
}

void Animation::reset()
{
	m_texture = m_start_texture;
	m_time_on_this_frame = 0;
}

TextureType Animation::get_texture_type()
{
	return m_texture;
}
