#include "grenade.h"

#include "character.h"
#include "projectile.h"
#include "functional"
#include "math_utils.h"
#include "graphics.h"

#include <iostream>

TextureType Grenade::m_projectile_texture = TextureType::GRANADE_10X10_1;
float Grenade::m_speed = 0.5;
int Grenade::m_life_time = 3000;
int Grenade::m_explosion_radius = 40;
int Grenade::m_projectile_width = 12;
int Grenade::m_projectile_height = 12;
int Grenade::m_hit_radius = 1;
float Grenade::m_gravity = 0.0008;
float Grenade::m_bounce = 0.4;

TextureType Grenade::m_explosion_texture = TextureType::EXPLOSION_40X40_4_FRAME1;
int Grenade::m_number_of_effect_frames = 11;
int Grenade::m_effect_size = 100;
int Grenade::m_effect_life_time = 200;

Grenade::Grenade() : Item{ 200 }
{
	m_left_weapon_texture = TextureType::GRENADE_LAUNCHER_LEFT_WEAPON;
	m_right_weapon_texture = TextureType::GRENADE_LAUNCHER_RIGHT_WEAPON;

	m_icon_image = TextureType::GRENADE_LAUNCHER_ICON;
}

Grenade::~Grenade()
{

}

void Grenade::use()
{
	if (is_off_cooldown())
	{
		m_cooldown = 0;

		recoil();
		muzzle_flash();

		Projectile* projectile = new Projectile(m_weapon_position.x + m_character->getForwardVector().x * 28,
												m_weapon_position.y + m_character->getForwardVector().y * 28,
												m_projectile_width, m_projectile_height, 
												m_character->getXVel() + (m_character->getForwardVector().x * m_speed), 
												(m_character->getForwardVector().y * m_speed),
												0, 
												10);

		projectile->set_texture_type(m_projectile_texture);

		projectile->set_projectile_type(Projectile_Type::BOUNCE);

		projectile->set_explosion_radius(m_explosion_radius);

		projectile->setRotation((m_character->getAim() * (M_PI / 180)));

		projectile->set_gravity(m_gravity);

		projectile->set_bounce(m_bounce);

		projectile->set_can_hit_players(false);

		projectile->set_life_time(m_life_time);

		projectile->set_effect(m_effect_size, m_effect_life_time, m_explosion_texture, m_effect_life_time / m_number_of_effect_frames, m_number_of_effect_frames);

		Game_Object::m_entities->push_front(projectile);
	}
}