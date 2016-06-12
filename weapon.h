#pragma once

#include "game_object.h"
#include "math_utils.h"
#include "graphics.h"

class Character;

class Weapon
{
public:
	Weapon();

	~Weapon();

	virtual void use();

	virtual void on_tick(unsigned int delta_time);

	virtual void draw();
	void draw_aim_cursor();

	//SETTERS & GETTERS
	void set_owner(Character* character);

	virtual vec2 get_weapon_correct_position();

	vec2 get_weapon_position();

	void on_start_use();

	TextureType get_item_icon_image();

protected:
	void draw_muzzle_flash();
	void recoil();
	void muzzle_flash();

	void damage_players();

	Character* m_character;

	TextureType m_left_weapon_texture;
	TextureType m_right_weapon_texture;
	TextureType m_active_weapon_texture;

	TextureType m_muzzle_flash_texture;

	float m_muzzle_flash_width;
	float m_muzzle_flash_ms_left;

	vec2 m_recoil_offset;
	int m_recoil_length;

	vec2 m_weapon_position;

	float m_weapon_rotation;

	TextureType m_aim_texture;
	int m_max_aim;
};

