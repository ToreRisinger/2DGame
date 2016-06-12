#pragma once

#include "graphics.h"

class Character;

class Skill
{
public:
	Skill(int cooldown, TextureType icon_image);
	~Skill();

	virtual void use();
	virtual void on_tick(int delta_time);

	bool is_off_cooldown();

	void set_owner(Character* owner);

	TextureType get_skill_icon_image();

protected:

	Character* m_owner;

	int m_skill_cooldown;
	int m_skill_current_cooldown;

	//ICON Image
	TextureType m_icon_image;
};

