#include "effect.h"

#include "graphics.h"
#include "game.h"

Effect::Effect(float x, float y, int size, int max_life_time, TextureType m_start_texture, int m_time_per_frame, int m_number_of_frames) : 
Positional_Game_Entity{ x, y, size, size }, m_animation(m_start_texture, m_time_per_frame, m_number_of_frames)
{
	m_life_time = 0;
	m_max_life_time = max_life_time;
	m_nr_of_frames = 0;

	m_z = 0.0;
}

Effect::~Effect()
{

}

void Effect::on_tick(unsigned int delta_time)
{
	m_life_time += delta_time;

	if (has_expired())
	{
		set_dead(true);
	}

	m_animation.on_tick(delta_time);
}

void Effect::draw()
{
	Game::graphics->render_texture(m_animation.get_texture_type(), m_x, m_y, -0.4, m_width, m_height, getRotation());
}


void Effect::set_max_life_time(int max_life_time)
{
	m_life_time = max_life_time;
}

int Effect::get_max_life_time()
{
	return m_max_life_time;
}

bool Effect::has_expired()
{
	if (m_life_time >= m_max_life_time)
	{
		return true;
	}

	return false;
}