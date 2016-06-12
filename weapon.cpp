#include "weapon.h"

#include "character.h"
#include "projectile.h"
#include "functional"
#include "math_utils.h"
#include "graphics.h"
#include "game.h"

Weapon::Weapon()
{
	m_left_weapon_texture = TextureType::STANDARD_LEFT_WEAPON;
	m_right_weapon_texture = TextureType::STANDARD_RIGHT_WEAPON;
	m_active_weapon_texture = m_right_weapon_texture;

	m_muzzle_flash_texture = TextureType::STANDARD_MUZZLE_FLASH;

	m_aim_texture = TextureType::AIM_1;

	m_muzzle_flash_width = 20;
	m_muzzle_flash_ms_left = 0;

	m_max_aim = 65;

	m_recoil_length = -4;

	m_recoil_offset.x = 0;
	m_recoil_offset.y = 0;
}

Weapon::~Weapon()
{

}

void Weapon::use()
{
	recoil();
	muzzle_flash();
}

void Weapon::on_tick(unsigned int delta_time)
{

	for (int i = 0; i < delta_time; i++)
	{
		//Weapon lag behind when moving
		m_weapon_position.x = get_weapon_correct_position().x;
		m_weapon_position.y = m_weapon_position.y * 0.95 + (get_weapon_correct_position().y) * 0.05;

		m_recoil_offset.x = m_recoil_offset.x * 0.95;
		m_recoil_offset.y = m_recoil_offset.y * 0.95;

	}

	//Muzzle flash
	if (m_muzzle_flash_ms_left > 0)
	{
		m_muzzle_flash_ms_left = m_muzzle_flash_ms_left - delta_time;
	}

	//Calculate weapon rotation
	vec2 forward_vector = m_character->getForwardVector();

	if (forward_vector.y < 0)
	{
		m_weapon_rotation = -acos(Normalize(forward_vector).x);
	}
	else
	{
		m_weapon_rotation = acos(Normalize(forward_vector).x);
	}

}

void Weapon::on_start_use()
{
	m_muzzle_flash_ms_left = 0;
	m_recoil_offset.x = 0;
	m_recoil_offset.y = 0;

	m_weapon_position.x = get_weapon_correct_position().x;
	m_weapon_position.y = get_weapon_correct_position().y;
}

void Weapon::draw()
{
	//Render weapon
	Direction character_direction = m_character->getDirection();

	Game::graphics->render_texture(TextureType(!(bool)character_direction * m_left_weapon_texture + (bool)character_direction * m_right_weapon_texture),
		m_weapon_position.x + m_character->getForwardVector().x * m_recoil_offset.x,
		m_weapon_position.y + m_character->getForwardVector().y * m_recoil_offset.y,
		- (0.2 * m_character->getDirection()),
		40,
		40,
		M_PI + m_weapon_rotation - M_PI * m_character->getDirection());

	draw_muzzle_flash();
}

void Weapon::draw_muzzle_flash()
{
	//Render muzzle flash
	if (m_muzzle_flash_ms_left > 0)
	{
		Game::graphics->render_texture(m_muzzle_flash_texture,
			m_weapon_position.x + m_character->getForwardVector().x * (28 + m_recoil_offset.x),
			m_weapon_position.y + m_character->getForwardVector().y * (28 + m_recoil_offset.y), 
			-0.5,
			m_muzzle_flash_width,
			m_muzzle_flash_width, 
			0);
	}

}


void Weapon::draw_aim_cursor()
{
	Game::graphics->render_texture(m_aim_texture,
		m_weapon_position.x + m_character->getForwardVector().x * 60,
		m_weapon_position.y + m_character->getForwardVector().y * 60, -0.5,
		24,
		24, 0);
}

void Weapon::set_owner(Character* character)
{
	m_character = character;
	m_weapon_position = get_weapon_correct_position();
}

void Weapon::recoil()
{
	m_recoil_offset.x = m_recoil_length;
	m_recoil_offset.y = m_recoil_length;
}

void Weapon::muzzle_flash()
{
	m_muzzle_flash_ms_left = 30;
}

vec2 Weapon::get_weapon_correct_position()
{
	return vec2{ m_character->getX() + 12.0f - 1.0f * m_character->getDirection(), m_character->getY() + 20.0f };
}


vec2 Weapon::get_weapon_position()
{
	return m_weapon_position;
}