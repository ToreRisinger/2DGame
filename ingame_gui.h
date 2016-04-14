#pragma once

#include "game_object.h"
#include "character.h"

class Ingame_GUI : public Game_Object
{
public:
	Ingame_GUI();
	~Ingame_GUI();

	void on_tick(unsigned int delta_time);
	void draw();

	void set_character(Character* character);


private:
	Character* m_character;

};

