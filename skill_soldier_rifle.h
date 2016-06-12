#pragma once

#include "skill.h"
#include "graphics.h"

class Skill_Soldier_Rifle : public Skill
{
public:
	Skill_Soldier_Rifle();
	~Skill_Soldier_Rifle();

	void use();

private:

	TextureType m_projectile_texture;
	float m_speed;
	int m_life_time;
	int m_explosion_radius;
	int m_projectile_width;
	int m_projectile_height;
	int m_hit_radius;

	TextureType m_explosion_texture;
	int m_number_of_effect_frames;
	int m_effect_size;
	int m_effect_life_time;
};

