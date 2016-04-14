#pragma once

#include "positional_game_entity.h"
#include "animation.h"

class Effect : public Positional_Game_Entity
{
public:
	Effect(float x, float y, int size, int max_life_time, TextureType m_start_texture, int m_time_per_frame, int m_number_of_frames);
	~Effect();

	void on_tick(unsigned int delta_time);
	void draw();

	void set_max_life_time(int max_life_time);

	int get_max_life_time();

	bool has_expired();

private:
	Animation m_animation;

	int m_max_life_time;

	int m_life_time;

	int m_nr_of_frames;
};

