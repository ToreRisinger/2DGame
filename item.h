#pragma once

#include "game_object.h"
#include "math_utils.h"
#include "graphics.h"

class Character;

class Item : public Game_Object
{
public:
	Item(int cooldown);
	~Item();

	virtual void use();

	virtual void on_tick(unsigned int delta_time);

	virtual void draw();
	void draw_aim_cursor();

	//SETTERS & GETTERS
	void set_owner(Character* character);

	void set_item_cooldown(int cooldown);
	int get_item_cooldown();

	int get_cooldown();

	bool is_off_cooldown();

	virtual vec2 get_weapon_correct_position();

	void on_start_use();

	TextureType get_item_icon_image();

protected:
	void draw_muzzle_flash();
	void recoil();
	void muzzle_flash();

	void damage_players();

	Character* m_character;

	int m_item_cooldown;

	int m_cooldown;

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

	//ICON Image
	TextureType m_icon_image;
};

