#include "game_object.h"

#include "graphics.h"
#include "game_level.h"
#include "game.h"
#include "player.h"

GameLevel* Game_Object::m_game_level = nullptr;
std::vector<Player*>* Game_Object::m_players = nullptr;
std::list<Game_Entity*>* Game_Object::m_entities = nullptr;
Graphics* Game_Object::m_graphics = nullptr;

Game_Object::Game_Object()
{

}

Game_Object::~Game_Object()
{

}

void Game_Object::set_game_level(GameLevel* game_level)
{
	m_game_level = game_level;
}

void Game_Object::set_players(std::vector<Player*>* players)
{
	m_players = players;
}

void Game_Object::set_game_entities(std::list<Game_Entity*>* entities)
{
	m_entities = entities;
}

void Game_Object::set_graphics(Graphics* graphics)
{
	m_graphics = graphics;
}
