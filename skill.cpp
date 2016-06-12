#include "skill.h"

#include "character.h"
#include <iostream>

Skill::Skill(int cooldown, TextureType icon_image)
{
	m_skill_cooldown = cooldown;
	m_skill_current_cooldown = 0;

	m_icon_image = icon_image;
}

Skill::~Skill()
{

}

void Skill::set_owner(Character* owner)
{
	m_owner = owner;
}

void Skill::use()
{

}

void Skill::on_tick(int delta_time)
{

	if (m_skill_current_cooldown < m_skill_cooldown)
	{
		m_skill_current_cooldown += delta_time;
	}

}

bool Skill::is_off_cooldown()
{
	return m_skill_current_cooldown >= m_skill_cooldown;
}

TextureType Skill::get_skill_icon_image() 
{
	return m_icon_image;
}