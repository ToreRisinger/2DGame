#include "game_entity.h"


Game_Entity::Game_Entity()
{
	m_is_dead = false;
}

Game_Entity::~Game_Entity()
{

}

void Game_Entity::on_tick(unsigned int delta_time)
{

}

void Game_Entity::draw()
{

}

bool Game_Entity::is_dead()
{
	return m_is_dead;
}

void Game_Entity::set_dead(bool value)
{
	m_is_dead = value;
}