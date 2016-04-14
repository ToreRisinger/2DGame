#pragma once

#include <vector>
#include <list>

class Game_Entity;
class Graphics;
class GameLevel;
class Player;

class Game_Object
{
public:
	Game_Object();
	~Game_Object();

	static void set_game_level(GameLevel* game_level);
	static void set_players(std::vector<Player*>* players);
	static void set_game_entities(std::list<Game_Entity*>* entities);
	static void set_graphics(Graphics* graphics);
	
	static Graphics* m_graphics;
	static GameLevel* m_game_level;
	static std::vector<Player*>* m_players;
	static std::list<Game_Entity*>* m_entities;
};

