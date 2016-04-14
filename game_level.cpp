#include "game_level.h"

#include <SDL/SDL_image.h>
#include <iostream>
#include <math.h>
#include <vector>

#include "debug_output.h"
#include "math_utils.h"
#include "graphics.h"
#include "particle_system.h"

GameLevel::GameLevel(float width, float height)
{
	m_texture_type = TextureType::NONE;

	m_width = width;
	m_height = height;

	int size = m_width * m_height;
	m_game_level = new LevelData[size];

	fillGameLevel();
}

GameLevel::~GameLevel()
{

}

void GameLevel::on_tick(unsigned int delta_time)
{

}

void GameLevel::draw()
{
	m_graphics->render_game_level(m_texture_type);
}
 

int getMin(int a, int b)
{
	if (a < b)
	{
		return a;
	}

	return b;
}

int getMax(int a, int b)
{
	if (a > b)
	{
		return a;
	}

	return b;
}

float GameLevel::getWidth()
{
	return m_width;
}

float GameLevel::getHeight()
{
	return m_height;
}

void GameLevel::destroyCircle(int x, int y, int radius)
{
	x = x / 4;
	y = y / 4;

	radius = radius / 4;

	int left = getMax(0, x - radius);
	int right = getMin(m_width, x + radius);

	int down = getMax(0,y - radius);
	int up = getMin(m_height, y + radius);

	vec3 point(x, y, 0);

	for (int yy = down; yy < up; yy++)
	{
		for (int xx = left; xx < right; xx++)
		{		
			if (Length(VectorSub(point, vec3(xx, yy, 0))) <= radius)
			{
				int index = xx + yy * m_width;

				m_game_level[index].data = 0x00;
			}
		}
	}

	m_changes.push(Rect{ 0, 0, m_width, m_height });

}

void GameLevel::destroyCircleWithParticles(int x, int y, int radius)
{
	std::vector<vec2> particle_positions;

	bool changed = false;

	int level_x = x / 4;
	int level_y = y / 4;

	radius = radius / 4;

	int left = getMax(0, level_x - radius);
	int right = getMin(m_width, level_x + radius);

	int down = getMax(0, level_y - radius);
	int up = getMin(m_height, level_y + radius);

	vec3 point(level_x, level_y, 0);

	for (int yy = down; yy < up; yy++)
	{
		for (int xx = left; xx < right; xx++)
		{
			if (Length(VectorSub(point, vec3(xx, yy, 0))) <= radius)
			{
				int index = xx + yy * m_width;
			
				if (!(m_game_level[index].data == 0x00))
				{
					particle_positions.push_back(vec2{ xx * 4.0f, yy * 4.0f });
					m_game_level[index].data = 0x00;
					changed = true;
				}		
			}
		}
	}

	if (changed) 
	{
		Particle_System* particle_system = new Particle_System(x, y, particle_positions, Game_Object::m_game_level->get_level_color(), 500, 0.001);
		Game_Object::m_entities->push_front(particle_system);

		m_changes.push(Rect{ 0, 0, m_width, m_height });
	}
}

void GameLevel::fillGameLevel()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int index = x + y * m_width;
			m_game_level[index].data = 0xFF;
		}
	}
}

void GameLevel::emptyGameLevel()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int index = x + y * m_width;
			m_game_level[index].data = 0x00;
		}
	}

}

LevelData* GameLevel::getGameLevelData()
{
	return m_game_level;
}

bool GameLevel::hasChanged() 
{
	return m_changes.size() > 0;
}

Rect GameLevel::getChange()
{
	Rect ret = m_changes.front();
	m_changes.pop();
	return ret;
}

bool GameLevel::isBlockSolid(int x, int y)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		return m_game_level[(int)(x + y * m_width)].data != 0x00;
	}

	return true;
}

bool GameLevel::isSolid(int x, int y)
{
	x = x / 4;
	y = y / 4;
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		return m_game_level[(int)(x + y * m_width)].data != 0x00;
	}
	
	return true;
}

bool GameLevel::isSolid(int x, int y, int radius)
{
	x = x / 4;
	y = y / 4;

	int start_x = x - radius;
	int start_y = y - radius;

	int end_x = x + radius;
	int end_y = y + radius;
	
	if (start_x < 0 || end_x > m_width || start_y < 0 || end_y > m_height)
	{
		return true;
	}

	for (int yy = start_y; yy < end_y; yy++)
	{
		for (int xx = start_x; xx < end_x; xx++)
		{
			if (m_game_level[(int)(xx + yy * m_width)].data != 0x00)
			{
				
				return true;
			} 
		}
	}

	return false;
}

vec3 GameLevel::get_level_color()
{
	return m_level_color;
}	

void GameLevel::set_texture_type(TextureType texture_type)
{
	m_texture_type = texture_type;

	if (m_texture_type == GAME_LEVEL_TEXTURE_DIRT)
	{
		m_level_color = vec3{ 0.698f, 0.384f, 0.172f };
	}
	else if (m_texture_type == GAME_LEVEL_TEXTURE_SAND)
	{
		m_level_color = vec3{ 1.0f, 0.87f, 0.50f };
	}
	else if (m_texture_type == GAME_LEVEL_TEXTURE_SNOW)
	{
		m_level_color = vec3{ 1.0f, 1.0f, 1.0f };
	}
	else if (m_texture_type == GAME_LEVEL_TEXTURE_DIRT_RED)
	{
		m_level_color = vec3{ 0.776f, 0.286f, 0.176f };
	}
	else if (m_texture_type == GAME_LEVEL_TEXTURE_DIRT_GREY)
	{
		m_level_color = vec3{ 0.6f, 0.6f, 0.6f };
	}
	else if (m_texture_type == GAME_LEVEL_TEXTURE_STONE)
	{
		m_level_color = vec3{ 0.7f, 0.7f, 0.7f };
	}
}