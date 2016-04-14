#include "Moving_Game_Entity.h"

Moving_Game_Entity::Moving_Game_Entity(float x, float y, int width, int height, float x_vel, float y_vel) 
: Positional_Game_Entity{ x, y, width, height }, m_x_vel{ x_vel }, m_y_vel{ y_vel }
{

}

Moving_Game_Entity::~Moving_Game_Entity()
{

}

float Moving_Game_Entity::getXVel()
{
	return m_x_vel;
}

void Moving_Game_Entity::setXVel(float x)
{
	m_x_vel = x;
}

float Moving_Game_Entity::getYVel()
{
	return m_y_vel;
}

void Moving_Game_Entity::setYVel(float y)
{
	m_y_vel = y;
}
