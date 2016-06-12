#include "character_soldier.h"

#include "skill_soldier_rifle.h"
#include "skill_soldier_rocket_launcher.h"
#include "skill_shovel.h"

Character_Soldier::Character_Soldier() : Character()
{
	//SET TEXTURE
	m_left_animation = Animation(TextureType::CHARACTER_SOLDIER_LEFT_FRAME1, 100, 4);
	m_right_animation = Animation(TextureType::CHARACTER_SOLDIER_RIGHT_FRAME1, 100, 4);
	m_active_animation = &m_right_animation;

	//ATTRIBUTES
	m_jump_speed = 0.3;
	m_speed = 0.15;

	m_max_hp = 100;
	m_current_hp = m_max_hp;

	Skill* skill_soldier_rifle = new Skill_Soldier_Rifle();
	skill_soldier_rifle->set_owner(this);
	m_skills.push_back(skill_soldier_rifle);

	Skill* skill_soldier_rocket_launcher = new Skill_Soldier_Rocket_Launcher();
	skill_soldier_rocket_launcher->set_owner(this);
	m_skills.push_back(skill_soldier_rocket_launcher);

	Skill* skill_shovel = new Skill_Shovel();
	skill_shovel->set_owner(this);
	m_skills.push_back(skill_shovel);
}

Character_Soldier::~Character_Soldier()
{

}
