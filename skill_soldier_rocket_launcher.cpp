#include "skill_soldier_rocket_launcher.h"

#include "projectile.h"
#include "game.h"
#include "character.h"
#include "weapon.h"

Skill_Soldier_Rocket_Launcher::Skill_Soldier_Rocket_Launcher() : Skill(100, TextureType::SOLDIER_ROCKET_LAUNCHER_ICON)
{

}

Skill_Soldier_Rocket_Launcher::~Skill_Soldier_Rocket_Launcher()
{

}

void Skill_Soldier_Rocket_Launcher::use() {
	
	m_skill_current_cooldown = 0;

	m_owner->get_weapon_model()->use();


	Projectile* projectile = new Projectile(m_owner->get_weapon_position().x + m_owner->getForwardVector().x * 28,
		m_owner->get_weapon_position().y + m_owner->getForwardVector().y * 28,
		m_projectile_width, m_projectile_height,
		m_owner->getForwardVector().x * m_speed,
		m_owner->getForwardVector().y * m_speed,
		5,
		5);

	projectile->set_texture_type(m_projectile_texture);

	projectile->set_explosion_radius(m_explosion_radius);

	projectile->setRotation((m_owner->getAim() * (M_PI / 180)));

	projectile->set_effect(m_effect_size, m_effect_life_time, m_explosion_texture, m_effect_life_time / m_number_of_effect_frames, m_number_of_effect_frames);

	Game::entities.push_front(projectile);
}
