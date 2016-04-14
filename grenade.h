#pragma once

#include "item.h"
#include "projectile.h"
#include "effect.h"

class Grenade : public Item
{
public:
	Grenade();
	~Grenade();

	void use();

private:

	static void on_death(Projectile* projectile);

	static TextureType m_projectile_texture;
	static float m_speed;
	static int m_life_time;
	static int m_explosion_radius;
	static int m_hit_radius;
	static int m_projectile_width;
	static int m_projectile_height;
	static float m_gravity;
	static float m_bounce;

	static TextureType m_explosion_texture;
	static int m_number_of_effect_frames;
	static int m_effect_size;
	static int m_effect_life_time;

};

