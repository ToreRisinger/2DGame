#pragma once

#include "positional_game_entity.h"

class Moving_Game_Entity : public Positional_Game_Entity
{
public:
	Moving_Game_Entity(float x, float y, int width, int height, float x_vel, float y_vel);
	~Moving_Game_Entity();

	float getXVel();
	void setXVel(float x);

	float getYVel();
	void setYVel(float y);

protected:

	float m_x_vel;
	float m_y_vel;
};

