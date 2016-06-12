#include "skill_soldier_rifle.h"

#include "projectile.h"
#include "game.h"
#include "character.h"
#include "weapon.h"

Skill_Soldier_Rifle::Skill_Soldier_Rifle() : Skill(60, TextureType::SOLDIER_RIFLE_ICON)
{
	m_projectile_texture = TextureType::BULLET_10X10_3;
	m_speed = 1.0;
	m_life_time = 3000;
	m_explosion_radius = 16;
	m_projectile_width = 8;
	m_projectile_height = 8;
	m_hit_radius = 1;

	m_explosion_texture = TextureType::EXPLOSION_40X40_3_FRAME1;
	m_number_of_effect_frames = 6;
	m_effect_size = 40;
	m_effect_life_time = 250;
}

Skill_Soldier_Rifle::~Skill_Soldier_Rifle()
{

}

void Skill_Soldier_Rifle::use()
{
	m_skill_current_cooldown = 0;

	m_owner->get_weapon_model()->use();

	Projectile* projectile = new Projectile(m_owner->get_weapon_position().x + m_owner->getForwardVector().x * 20,
		m_owner->get_weapon_position().y + m_owner->getForwardVector().y * 20,
		m_projectile_width,
		m_projectile_height,
		m_owner->getForwardVector().x * m_speed,
		m_owner->getForwardVector().y * m_speed,
		2,
		0);

	projectile->set_texture_type(m_projectile_texture);

	projectile->set_explosion_radius(m_explosion_radius);

	projectile->setRotation((m_owner->getAim() * (M_PI / 180)));

	projectile->set_effect(m_effect_size, m_effect_life_time, m_explosion_texture, m_effect_life_time / m_number_of_effect_frames, m_number_of_effect_frames);

	projectile->set_owner(m_owner);

	projectile->set_can_hit_owner(false);

	Game::entities.push_front(projectile);
}
