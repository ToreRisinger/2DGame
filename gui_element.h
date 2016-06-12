#pragma once

#include "math_utils.h"
#include "game_object.h"

class GUI_Element : public Game_Object
{
public:
	GUI_Element(int x, int y, int width, int height);
	~GUI_Element();

	virtual void on_tick(unsigned int delta_time);
	virtual void draw();
	virtual void handle_input();

	virtual void reset();

protected:


	int m_x;
	int m_y;

	int m_width;
	int m_height;



};

