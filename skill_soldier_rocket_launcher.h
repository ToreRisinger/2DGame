#pragma once

#include "skill.h"
#include "graphics.h"

class Skill_Soldier_Rocket_Launcher : public Skill
{
public:
	Skill_Soldier_Rocket_Launcher();
	~Skill_Soldier_Rocket_Launcher();

	void use();

private:
	TextureType m_projectile_texture = TextureType::BULLET_10X10_1;
	float m_speed = 0.6;
	int m_life_time = 10000;
	int m_explosion_radius = 32;
	int m_projectile_width = 8;
	int m_projectile_height = 8;
	int m_hit_radius = 1;

	TextureType m_explosion_texture = TextureType::EXPLOSION_40X40_4_FRAME1;
	int m_number_of_effect_frames = 11;
	int m_effect_size = 80;
	int m_effect_life_time = 200;
};

