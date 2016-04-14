#include "item.h"

#include "character.h"

#include <iostream>


Item::Item(int cooldown) : m_item_cooldown{ cooldown }, m_cooldown{ 0 }
{
	m_left_weapon_texture = TextureType::STANDARD_LEFT_WEAPON;
	m_right_weapon_texture = TextureType::STANDARD_RIGHT_WEAPON;
	m_active_weapon_texture = m_right_weapon_texture;

	m_muzzle_flash_texture = TextureType::STANDARD_MUZZLE_FLASH;

	m_aim_texture = TextureType::AIM_1;

	m_muzzle_flash_width = 20;
	m_muzzle_flash_ms_left = 0;

	m_max_aim = 65;

	m_recoil_length = -3;

	m_recoil_offset.x = 0;
	m_recoil_offset.y = 0;

	m_icon_image = TextureType::RIFLE_ICON;
}

Item::~Item()
{

}

void Item::use()
{
	
}

void Item::on_tick(unsigned int delta_time)
{
	if (m_cooldown < m_item_cooldown)
	{
		m_cooldown += delta_time;
	}
	
	for (int i = 0; i < delta_time; i++)
	{	
		//Weapon lag behind when moving
		m_weapon_position.x = get_weapon_correct_position().x;
		m_weapon_position.y = m_weapon_position.y * 0.95 + (get_weapon_correct_position().y) * 0.05;

		m_recoil_offset.x = m_recoil_offset.x * 0.99;
		m_recoil_offset.y = m_recoil_offset.y * 0.99;
		
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

void Item::on_start_use()
{
	m_muzzle_flash_ms_left = 0;
	m_recoil_offset.x = 0;
	m_recoil_offset.y = 0;

	m_weapon_position.x = get_weapon_correct_position().x;
	m_weapon_position.y = get_weapon_correct_position().y;
}

void Item::draw()
{	
	//Render weapon
	Direction character_direction = m_character->getDirection();

	Game_Object::m_graphics->render_texture(TextureType(!(bool)character_direction * m_left_weapon_texture + (bool)character_direction * m_right_weapon_texture),
											m_weapon_position.x + m_character->getForwardVector().x * m_recoil_offset.x,
											m_weapon_position.y + m_character->getForwardVector().y * m_recoil_offset.y,
											-0.0 - (0.2 * m_character->getDirection()), 
											40, 
											40, 
											M_PI + m_weapon_rotation - M_PI * m_character->getDirection());

	draw_muzzle_flash();
}

void Item::draw_muzzle_flash()
{
	//Render muzzle flash
	if (m_muzzle_flash_ms_left > 0)
	{
		Game_Object::m_graphics->render_texture(m_muzzle_flash_texture,
												m_weapon_position.x + m_character->getForwardVector().x * (28 + m_recoil_offset.x),
												m_weapon_position.y + m_character->getForwardVector().y * (28 + m_recoil_offset.y), -0.5,
												m_muzzle_flash_width,
												m_muzzle_flash_width, 0);
	}

}


void Item::draw_aim_cursor()
{
	Game_Object::m_graphics->render_texture(m_aim_texture,
											m_weapon_position.x + m_character->getForwardVector().x * 60,
											m_weapon_position.y + m_character->getForwardVector().y * 60, -0.5,
											24,
											24, 0);
}

void Item::set_owner(Character* character)
{
	m_character = character;
	m_weapon_position = get_weapon_correct_position();
}

void Item::set_item_cooldown(int skill_cooldown)
{
	m_item_cooldown = skill_cooldown;
}

int Item::get_item_cooldown()
{
	return m_item_cooldown;
}

int Item::get_cooldown()
{
	return m_cooldown;
}

bool Item::is_off_cooldown()
{
	return m_cooldown >= m_item_cooldown;
}

void Item::recoil()
{
	m_recoil_offset.x = m_recoil_offset.x + m_recoil_length;
	m_recoil_offset.y = m_recoil_offset.y + m_recoil_length;
}

void Item::muzzle_flash()
{
	m_muzzle_flash_ms_left = m_muzzle_flash_ms_left + 60;
}

vec2 Item::get_weapon_correct_position()
{
	return vec2{ m_character->getX() + 12.0f - 2.0f * m_character->getDirection(), m_character->getY() + 20.0f };
}

TextureType Item::get_item_icon_image()
{
	return m_icon_image;
}