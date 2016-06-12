#pragma once

#undef main
#include <GL/glew.h>
#include <GL/GL.h>

#include <string>
#include <SDL/SDL.h>
#include <queue>

#include "game_object.h"
#include "vertex.h"
#include "math_utils.h"

enum TextureType;

struct LevelData {
	char data;
};

struct Rect {
	int x;
	int y;
	int width;
	int height;
};

struct Game_Level_Option {
	int width;
	int height;
	TextureType dirt_type;

};

class GameLevel : public Game_Object
{
public:
	GameLevel();
	~GameLevel();

	void setup_new_game_level(Game_Level_Option options);

	void on_tick(unsigned int delta_time);
	void draw();

	float getWidth();
	float getHeight();

	void destroyCircle(int x, int y, int radius);
	void destroyCircleWithParticles(int x, int y, int radius);
	void fillGameLevel();
	void emptyGameLevel();

	LevelData* getGameLevelData();
	Rect getChange();
	bool hasChanged();

	bool isSolid(int x, int y);
	bool isSolid(int x, int y, int radius);

	bool isBlockSolid(int x, int y);

	vec3 get_level_color();

	void set_texture_type(TextureType texture_type);

private:
	void clean_up();

	std::queue<Rect> m_changes;

	float m_width;
	float m_height;

	vec3 m_level_color;

	TextureType m_texture_type;

	LevelData* m_game_level;
};

