#include "rocket_launcher.h"

#include "character.h"
#include "projectile.h"
#include "functional"
#include "math_utils.h"
#include "graphics.h"
#include "game.h"

TextureType Rocket_Launcher::m_projectile_texture = TextureType::BULLET_10X10_1;
float Rocket_Launcher::m_speed = 0.6;
int Rocket_Launcher::m_life_time = 10000;
int Rocket_Launcher::m_explosion_radius = 32;
int Rocket_Launcher::m_projectile_width = 8;
int Rocket_Launcher::m_projectile_height = 8;
int Rocket_Launcher::m_hit_radius = 1;

TextureType Rocket_Launcher::m_explosion_texture = TextureType::EXPLOSION_40X40_4_FRAME1;
int Rocket_Launcher::m_number_of_effect_frames = 11;
int Rocket_Launcher::m_effect_size = 80;
int Rocket_Launcher::m_effect_life_time = 200;

Rocket_Launcher::Rocket_Launcher() : Item{ 100 }
{

}

Rocket_Launcher::~Rocket_Launcher()
{

}

void Rocket_Launcher::use()
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
												5);
		
		projectile->set_texture_type(m_projectile_texture);

		projectile->set_explosion_radius(m_explosion_radius);

		projectile->setRotation((m_character->getAim() * (M_PI / 180)));

		projectile->set_effect(m_effect_size, m_effect_life_time, m_explosion_texture, m_effect_life_time / m_number_of_effect_frames, m_number_of_effect_frames);

		Game::entities.push_front(projectile);
	}

}
