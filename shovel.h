#pragma once

#include "item.h"

class Shovel: public Item
{
public:
	Shovel();
	~Shovel();

	void use();
	void draw();

	vec2 get_weapon_correct_position();

private:
	int m_range;
	int m_radius;
};

