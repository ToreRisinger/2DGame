#pragma once

#include "game_object.h"

class Game_Entity : public Game_Object
{
public:
	Game_Entity();
	virtual ~Game_Entity();

	virtual void on_tick(unsigned int delta_time);
	virtual void draw();

	bool is_dead();
	void set_dead(bool value);

protected:
	bool m_is_dead;

};

