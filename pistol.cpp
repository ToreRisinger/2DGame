#include "pistol.h"

#include "character.h"
#include "projectile.h"
#include "functional"
#include "math_utils.h"
#include "graphics.h"

#include <iostream>

TextureType Pistol::m_projectile_texture = TextureType::BULLET_10X10_2;
float Pistol::m_speed = 1.0;
int Pistol::m_life_time = 3000;
int Pistol::m_explosion_radius = 20;
int Pistol::m_projectile_width = 8;
int Pistol::m_projectile_height = 8;
int Pistol::m_hit_radius = 1;

TextureType Pistol::m_explosion_texture = TextureType::EXPLOSION_40X40_3_FRAME1;
int Pistol::m_number_of_effect_frames = 6;
int Pistol::m_effect_size = 48;
int Pistol::m_effect_life_time = 250;

Pistol::Pistol() : Item{ 600 }
{

}

Pistol::~Pistol()
{

}

void Pistol::use()
{
	if (is_off_cooldown())
	{
		m_cooldown = 0;

		recoil();

		muzzle_flash();

		Projectile* projectile = new Projectile(m_weapon_position.x + m_character->getForwardVector().x * 28,
												m_weapon_position.y + m_character->getForwardVector().y * 28,
												m_projectile_width, m_projectile_height,
												m_character->getForwardVector().x * m_speed,
												m_character->getForwardVector().y * m_speed,
												5,
												0);

		projectile->set_texture_type(m_projectile_texture);

		projectile->set_explosion_radius(m_explosion_radius);

		projectile->set_effect(m_effect_size, m_effect_life_time, m_explosion_texture, m_effect_life_time / m_number_of_effect_frames, m_number_of_effect_frames);

		projectile->setRotation((m_character->getAim() * (M_PI / 180)));

		Game_Object::m_entities->push_front(projectile);
	}
}
