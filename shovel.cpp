#include "shovel.h"

#include "character.h"
#include "game_level.h"
#include "particle_system.h"
#include "game.h"

Shovel::Shovel() : Item{ 200 }
{
	m_range = 24;
	m_radius = 32;

	m_left_weapon_texture = TextureType::SHOVEL_LEFT;
	m_right_weapon_texture = TextureType::SHOVEL_RIGHT;

	m_recoil_length = 12;

	m_icon_image = TextureType::SHOVEL_ICON;
}

Shovel::~Shovel()
{

}

void Shovel::use()
{
	if (is_off_cooldown())
	{
		recoil();

		m_cooldown = 0;

		int x = m_character->getX() + (m_character->getWidth() / 2) + m_character->getForwardVector().x * m_range;
		int y = m_character->getY() + (m_character->getHeight() / 2) + m_character->getForwardVector().y * m_range;

		Game::game_level->destroyCircleWithParticles(x, y, m_radius);

	}
}

void Shovel::draw()
{
	//Render weapon
	Direction character_direction = m_character->getDirection();

	Game::graphics->render_texture(TextureType(!(bool)character_direction * m_left_weapon_texture + (bool)character_direction * m_right_weapon_texture),
											m_weapon_position.x + m_character->getForwardVector().x * m_recoil_offset.x,
											m_weapon_position.y + m_character->getForwardVector().y * m_recoil_offset.y,
											-0.1 - (0.2 * m_character->getDirection()),
											50,
											50,
											M_PI + m_weapon_rotation - M_PI * m_character->getDirection());

}

vec2 Shovel::get_weapon_correct_position()
{
	return vec2{ m_character->getX() + 12.0f - 2.0f * m_character->getDirection(), m_character->getY() + 16.0f };
}