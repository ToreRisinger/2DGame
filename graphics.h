#pragma once

#include <Windows.h>
#include <SDL/SDL.h>
#undef main
#include <GL/glew.h>
#include <GL/GL.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <list>
#include <map>

#include <SDL/SDL_image.h>

#include "shader_program.h"
#include "math_utils.h"
#include "player.h"
#include "game_level.h"
#include "vertex.h"
#include "game_entity.h"

const float GRAPHIC_SCALE = 4;

const int NR_OF_TEXTURES = 100;

enum TextureType {
	NONE,

	//PROJECTILES
	BULLET_10X10_1,
	BULLET_10X10_2,
	BULLET_10X10_3,
	BULLET_10X10_4,
	BULLET_10X10_5,
	GRANADE_10X10_1,
	GRANADE_10X10_2,

	//EXPLOSIONS
	EXPLOSION_40X40_1,
	EXPLOSION_40X40_2,
	EXPLOSION_40X40_3_FRAME1,
	EXPLOSION_40X40_3_FRAME2,
	EXPLOSION_40X40_3_FRAME3,
	EXPLOSION_40X40_3_FRAME4,
	EXPLOSION_40X40_3_FRAME5,
	EXPLOSION_40X40_3_FRAME6,
	EXPLOSION_40X40_4_FRAME1,
	EXPLOSION_40X40_4_FRAME2,
	EXPLOSION_40X40_4_FRAME3,
	EXPLOSION_40X40_4_FRAME4,
	EXPLOSION_40X40_4_FRAME5,
	EXPLOSION_40X40_4_FRAME6,
	EXPLOSION_40X40_4_FRAME7,
	EXPLOSION_40X40_4_FRAME8,
	EXPLOSION_40X40_4_FRAME9,
	EXPLOSION_40X40_4_FRAME10,
	EXPLOSION_40X40_4_FRAME11,

	//GAME LEVEL
	GAME_LEVEL_TEXTURE_DIRT,
	GAME_LEVEL_TEXTURE_DIRT_RED,
	GAME_LEVEL_TEXTURE_DIRT_GREY,
	GAME_LEVEL_TEXTURE_SNOW,
	GAME_LEVEL_TEXTURE_SAND,
	GAME_LEVEL_TEXTURE_STONE,

	//CHARACTER TEXTURES
	CHARACTER_SOLDIER_RIGHT_FRAME1,
	CHARACTER_SOLDIER_RIGHT_FRAME2,
	CHARACTER_SOLDIER_RIGHT_FRAME3,
	CHARACTER_SOLDIER_RIGHT_FRAME4,
	CHARACTER_SOLDIER_LEFT_FRAME1,
	CHARACTER_SOLDIER_LEFT_FRAME2,
	CHARACTER_SOLDIER_LEFT_FRAME3,
	CHARACTER_SOLDIER_LEFT_FRAME4,

	//AIM
	AIM_1,

	//WEAPONS
	STANDARD_LEFT_WEAPON,
	STANDARD_RIGHT_WEAPON,

	GRENADE_LAUNCHER_LEFT_WEAPON,
	GRENADE_LAUNCHER_RIGHT_WEAPON,

	//WEAPON MUZZLE FLASH
	STANDARD_MUZZLE_FLASH,

	SHOVEL_LEFT,
	SHOVEL_RIGHT,

	//HP BAR
	HP_BAR,

	//ICON
	SOLDIER_RIFLE_ICON,
	SOLDIER_ROCKET_LAUNCHER_ICON,
	SHOVEL_ICON,

	//BUTTONS
	BUTTON_1_128X64_IDLE,
	BUTTON_1_128X64_HIGHLIGHTED,
	RADIO_BUTTON_1_32X32_NOT_SELECTED,
	RADIO_BUTTON_1_32X32_SELECTED
};

/// Holds all state information relevant to a character as loaded using FreeType
struct Font_Character {
    GLuint textureID;   // ID handle of the glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
    GLuint advance;    // Horizontal offset to advance to next glyph
};

enum Text_Textures {
	MENU_BUTTON_TEXT_MULTIPLAYER,
	MENU_BUTTON_TEXT_SINGLEPLAYER,
	MENU_BUTTON_TEXT_LOADOUT,
	MENU_BUTTON_TEXT_EXIT,
	MENU_BUTTON_TEXT_START_GAME,
	MENU_BUTTON_TEXT_BACK,
	MENU_BUTTON_TEXT_RESUME
};

class Graphics
{
public:
	Graphics(std::string window_title, int window_width, int window_height, GameLevel* game_level, std::list<Game_Entity*>* entities, std::vector<Player*>* players);
	~Graphics();

	void clearScreen();
	void swapWindow();

	//Render texture
	void render_texture(TextureType texture, float x, float y, float z, float width, float height, float rot);
	void render_texture_ui(TextureType texture, float x, float y, float width, float height, float rot);

	void render_dynamic_text(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

	//Render game level
	void render_game_level(TextureType texture);

	//Render particle
	void render_particles(int number_of_particles, int particle_system_id, vec3 color);
	void update_particles(vec3* particle_positions, int number_of_particles, int particle_system_id);

	//-----HELP FUNCTIONS-----
	vec2 getOpenGLCoordinates(float x, float y);
	void setCameraPosition(float x, float y);
	vec2 getCameraPosition();

	int getParticleSystemID();
	void returnParticleSystemID(int id);

	void init_game_level_graphics();
private:
	void init_systems(std::string window_title, int window_width, int window_height);

	//-----VARIABLES-----
	SDL_Window* _window;

	float m_camera_x;
	float m_camera_y;

	float m_window_width;
	float m_window_height;

	//-----MATRICES-----
	mat4 m_cam_translation;
	mat4 m_translation;
	mat4 m_scale;
	mat4 m_projection;

	//-----REFERENCES-----
	GameLevel* m_game_level;
	std::list<Game_Entity*>* m_entities;
	std::vector<Player*>* m_players;

	//-----SPRITE SHEET-----
	void load_sprite_sheet();
	void load_textures_types();

	void blitSurface(SDL_Surface* source, SDL_Surface* destination, int clip_x, int clip_y, int clip_width, int clip_height);

	SDL_Surface* m_sprite_sheet;

	GLuint* m_texture_id_array;

	//-----PARTICLE GRAPHICS-----
	void init_particle_graphics();

	ShaderProgram* m_shader_program_particle;

	int m_max_particles;
	int m_max_particles_per_system;
	int m_number_of_particle_system_ids;

	bool* m_occupied_particle_system_ID;

	vec3 m_particle_vertex[6];

	GLuint m_particle_vao_ID;
	GLuint m_particle_vertex_buffer_ID;
	GLuint m_particle_position_buffer_ID;

	//----TEXTURE GRAPHICS-----
	void init_texture_graphics();

	void load_texture(int texture_index, int texture_width, int texture_height, int clip_x, int clip_y, int clip_width, int clip_height);

	ShaderProgram* m_shader_program_texture;

	Vertex m_texture_vertex[6];
	GLuint m_texture_vao_ID;
	GLuint m_texture_vbo_ID;

	//-----GAME LEVEL GRAPHICS-----
	

	void updateGameLevelMaskTextureInOpenGL();

	ShaderProgram* m_shader_program_game_level;

	Vertex m_game_level_vertex[6];
	GLuint m_game_level_vao_ID;
	GLuint m_game_level_vbo_ID;

	GLuint m_game_level_texture_id;
	GLuint m_game_level_mask_texture_id;

	float m_map_texture_width;
	float m_map_texture_height;

	//----FONT GRAPHICS-----
	void init_font_graphics();
	std::map<GLchar, Font_Character> m_characters;

	GLuint m_font_vao_ID;
	GLuint m_font_vbo_ID;

	ShaderProgram* m_shader_program_font;

	//----STATIC TEXT GRAPHICS-----
	
};

