#include "positional_game_entity.h"

#include <iostream>

#include "graphics.h"

Positional_Game_Entity::Positional_Game_Entity(float x, float y, int width, int height) : m_x{ x }, m_y{ y }, m_z{ -1.0 }, m_width{ width }, m_height{ height }
{
	m_rotation = 0;
	m_visible = true;
}

Positional_Game_Entity::~Positional_Game_Entity()
{
	
}

void Positional_Game_Entity::on_tick(unsigned int delta_time)
{

}

void Positional_Game_Entity::draw()
{

}

void Positional_Game_Entity::on_death()
{

}

float Positional_Game_Entity::getX()
{
	return m_x;
}

void Positional_Game_Entity::setX(float x)
{
	m_x = x;
}

float Positional_Game_Entity::getY()
{
	return m_y;
}

void Positional_Game_Entity::setY(float y)
{
	m_y = y;
}

float Positional_Game_Entity::getZ()
{
	return m_z;
}

void Positional_Game_Entity::setZ(float z)
{
	m_z = z;
}

float Positional_Game_Entity::getWidth()
{
	return m_width;
}

void Positional_Game_Entity::setWidth(float width)
{
	m_width = width;
}

float Positional_Game_Entity::getHeight()
{
	return m_height;
}

void Positional_Game_Entity::setHeight(float height)
{
	m_height = height;
}

float Positional_Game_Entity::getRotation()
{
	return m_rotation;
}

void Positional_Game_Entity::setRotation(float rotation)
{
	m_rotation = rotation;
}

bool Positional_Game_Entity::isVisible()
{
	return m_visible;
}

void Positional_Game_Entity::setVisible(bool value)
{
	m_visible = value;
}