#include "skill_shovel.h"

#include "game.h"
#include "character.h"

Skill_Shovel::Skill_Shovel() : Skill(200, TextureType::SHOVEL_ICON)
{

}

Skill_Shovel::~Skill_Shovel()
{

}

void Skill_Shovel::use() 
{
	m_skill_current_cooldown = 0;
	
	int range = 24;
	int radius = 32;

	int x = m_owner->getX() + (m_owner->getWidth() / 2) + m_owner->getForwardVector().x * range;
	int y = m_owner->getY() + (m_owner->getHeight() / 2) + m_owner->getForwardVector().y * range;

	Game::game_level->destroyCircleWithParticles(x, y, radius);
}
