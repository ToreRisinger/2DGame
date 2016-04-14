#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/GL.h>
#include <functional>

#include "game_entity.h"

class GameLevel;
class Player;
class Logic;

enum TextureType;

class Positional_Game_Entity : public Game_Entity
{
public:
	Positional_Game_Entity(float x, float y, int width, int height);
	virtual ~Positional_Game_Entity();

	virtual void on_tick(unsigned int delta_time);
	virtual void draw();
	virtual void on_death();

	virtual float getX();
	virtual void setX(float x);

	virtual float getY();
	virtual void setY(float y);

	virtual float getZ();
	virtual void setZ(float z);

	virtual float getWidth();
	virtual void setWidth(float width);

	virtual float getHeight();
	virtual void setHeight(float height);

	virtual float getRotation();
	virtual void setRotation(float rotation);

	virtual bool isVisible();
	virtual void setVisible(bool value);

protected:
	//VARIABLES
	float m_rotation;

	float m_x;
	float m_y;
	float m_z;

	float m_width;
	float m_height;

	bool m_visible;
};

