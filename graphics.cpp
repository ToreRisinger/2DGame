#include "graphics.h"

#include "vertex.h"
#include "debug_output.h"
#include "character.h"
#include "vertex.h"
#include "game_level.h"
#include "particle_system.h"

#include <math.h>
#include <iostream>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

Graphics::Graphics(std::string window_title, int window_width, int window_height, GameLevel* game_level, std::list<Game_Entity*>* entities, std::vector<Player*>* players)
{
	//REFERENCES
	m_game_level = game_level;
	m_players = players;
	m_entities = entities;

	//VARIABLES
	m_window_width = window_width;
	m_window_height = window_height;

	m_camera_x = 0;
	m_camera_y = 0;

	//INIT OPENGL
	init_systems(window_title, window_width, window_height);

	//SHADER PROGRAMS
	m_shader_program_game_level = new ShaderProgram();
	m_shader_program_game_level->compileShaders("shaders/shader_game_level.vert", "shaders/shader_game_level.frag");
	m_shader_program_game_level->linkShaders();

	m_shader_program_texture = new ShaderProgram();
	m_shader_program_texture->compileShaders("shaders/shader_texture.vert", "shaders/shader_texture.frag");
	m_shader_program_texture->linkShaders();

	m_shader_program_particle = new ShaderProgram();
	m_shader_program_particle->compileShaders("shaders/shader_particle.vert", "shaders/shader_particle.frag");
	m_shader_program_particle->linkShaders();

	m_shader_program_font = new ShaderProgram();
	m_shader_program_font->compileShaders("shaders/shader_font.vert", "shaders/shader_font.frag");
	m_shader_program_font->linkShaders();

	m_texture_id_array = new GLuint[NR_OF_TEXTURES];

	//SPRITE SHEEt
	load_sprite_sheet();

	load_textures_types();

	//GAME LEVEL
	//init_game_level_graphics();

	//TEXTURE
	init_texture_graphics();

	//PARTICLE GRAPHICS
	init_particle_graphics();

	//FONT GRAPHICS
	init_font_graphics();

	m_projection = frustum(0, m_window_width - 1, 0, m_window_height - 1, 1.0, -1.0);

}

Graphics::~Graphics()
{

}

void Graphics::init_systems(std::string window_title, int window_width, int window_height)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);

	if (_window == nullptr)
	{
		error_output("SDL Window could not be created!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);

	if (glContext == nullptr)
	{
		error_output("SDL_GL context could not be created!");
	}

	glewExperimental = GL_TRUE;
	GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		error_output("Could not initialize glew");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Use Vsync
	if (SDL_GL_SetSwapInterval(0) < 0)
	{
		std::string s = SDL_GetError();
		error_output("Unable to set VSync! SDL Error: " + s);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0);

	//SDL_ShowCursor(0);
	SDL_ShowCursor(1);

	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Graphics::init_game_level_graphics()
{
	//---UPLOAD MASK---
	glGenTextures(1, &m_game_level_mask_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_game_level_mask_texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_game_level->getWidth(), m_game_level->getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, (void*)m_game_level->getGameLevelData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//---CREATE GAME LEVEL QUAD---
	float vertex_width = 1.0f;
	float vertex_height = 1.0f;

	float texture_coord_width = m_game_level->getWidth() / 80;
	float texture_coord_height = m_game_level->getHeight() / 80;

	m_game_level_vertex[0] = { { 0, 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
	m_game_level_vertex[1] = { { 0, vertex_height, 0 }, { 0, texture_coord_height }, { 0, 1 }, { 0, 1 } };
	m_game_level_vertex[2] = { { vertex_width, vertex_height, 0 }, { texture_coord_width, texture_coord_height }, { 1, 1 }, { 1, 1 } };
	m_game_level_vertex[3] = { { 0, 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
	m_game_level_vertex[4] = { { vertex_width, vertex_height, 0.0 }, { texture_coord_width, texture_coord_height }, { 1, 1 }, { 1, 1 } };
	m_game_level_vertex[5] = { { vertex_width, 0, 0 }, { texture_coord_width, 0 }, { 1, 0 }, { 1, 0 } };

	//---UPLOAD MAP TO SHADER---
	glGenVertexArrays(1, &m_game_level_vao_ID);
	glGenBuffers(1, &m_game_level_vbo_ID);
	glBindVertexArray(m_game_level_vao_ID);

	glBindBuffer(GL_ARRAY_BUFFER, m_game_level_vbo_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * 6, m_game_level_vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (const GLfloat*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (const GLfloat*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (const GLfloat*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (const GLfloat*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
}

void Graphics::init_texture_graphics()
{
	float texture_coord_width = 1;
	float texture_coord_height = 1;

	m_texture_vertex[0].position = { -0.5, -0.5, 0 };
	m_texture_vertex[0].texture_coord = { 0, 0 };

	m_texture_vertex[1].position = { -0.5, 0.5, 0 };
	m_texture_vertex[1].texture_coord = { 0, texture_coord_height };

	m_texture_vertex[2].position = { 0.5, 0.5, 0 };
	m_texture_vertex[2].texture_coord = { texture_coord_width, texture_coord_height };

	m_texture_vertex[3].position = { -0.5, -0.5, 0 };
	m_texture_vertex[3].texture_coord = { 0, 0 };

	m_texture_vertex[4].position = { 0.5, 0.5, 0 };
	m_texture_vertex[4].texture_coord = { texture_coord_width, texture_coord_height };

	m_texture_vertex[5].position = { 0.5, -0.5, 0 };
	m_texture_vertex[5].texture_coord = { texture_coord_width, 0 };


	glGenVertexArrays(1, &m_texture_vao_ID);
	glGenBuffers(1, &m_texture_vbo_ID);
	glBindVertexArray(m_texture_vao_ID);

	glBindBuffer(GL_ARRAY_BUFFER, m_texture_vbo_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * 6, m_texture_vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (const GLfloat*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (const GLfloat*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void Graphics::init_particle_graphics()
{
	//PARTICLE
	m_max_particles = 1000;
	m_max_particles_per_system = 20;
	m_number_of_particle_system_ids = m_max_particles / m_max_particles_per_system;

	m_occupied_particle_system_ID = new bool[m_number_of_particle_system_ids];

	for (int i = 0; i < m_number_of_particle_system_ids; i++)
	{
		m_occupied_particle_system_ID[i] = false;
	}

	m_particle_vertex[0] = { -0.5, -0.5, -0.5 };
	m_particle_vertex[1] = { -0.5, 0.5, -0.5 };
	m_particle_vertex[2] = { 0.5, 0.5, -0.5 };
	m_particle_vertex[3] = { -0.5, -0.5, -0.5 };
	m_particle_vertex[4] = { 0.5, 0.5, -0.5 };
	m_particle_vertex[5] = { 0.5, -0.5, -0.5 };

	glGenVertexArrays(1, &m_particle_vao_ID);
	glBindVertexArray(m_particle_vao_ID);

	//vertex buffer
	glGenBuffers(1, &m_particle_vertex_buffer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_particle_vertex_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_particle_vertex), m_particle_vertex, GL_STATIC_DRAW);

	//position buffer
	glGenBuffers(1, &m_particle_position_buffer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_particle_position_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER, m_max_particles * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_particle_vertex_buffer_ID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_particle_position_buffer_ID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
}

void Graphics::clearScreen()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
}

void Graphics::render_game_level(TextureType texture)
{	
	m_shader_program_game_level->use();

	//Update map
	updateGameLevelMaskTextureInOpenGL();

	GLint textureUniform;
	textureUniform = glGetUniformLocation(m_shader_program_game_level->getProgramID(), "game_level_mask_texture_sampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_game_level_mask_texture_id);

	textureUniform = glGetUniformLocation(m_shader_program_game_level->getProgramID(), "game_level_texture_sampler");
	glUniform1i(textureUniform, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture_id_array[texture]);

	//Translation
	m_translation = T(0, 0, -0.5);
	glUniformMatrix4fv(m_shader_program_game_level->getUniformLocation("translation"), 1, GL_FALSE, Transpose(m_translation).m);

	//Cam Translation
	m_cam_translation = T(-(int)m_camera_x, -(int)m_camera_y, 0);
	glUniformMatrix4fv(m_shader_program_game_level->getUniformLocation("cam_translation"), 1, GL_FALSE, Transpose(m_cam_translation).m);

	//scale
	mat4 scale = S(800 * 4, 400 * 4, 0);
	glUniformMatrix4fv(m_shader_program_game_level->getUniformLocation("scale"), 1, GL_FALSE, Transpose(scale).m);

	//Translation
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("projection"), 1, GL_FALSE, Transpose(m_projection).m);

	//Render
	glBindVertexArray(m_game_level_vao_ID);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_shader_program_game_level->unuse();

	glActiveTexture(GL_TEXTURE0);
}

void Graphics::render_texture(TextureType texture, float x, float y, float z, float width, float height, float rot)
{
	glEnable(GL_DEPTH_TEST);

	m_shader_program_texture->use();

	GLint textureUniform;
	textureUniform = glGetUniformLocation(m_shader_program_texture->getProgramID(), "texture_sampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_id_array[texture]);

	//Translation
	m_translation = T((int)x, (int)y, z);
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("translation"), 1, GL_FALSE, Transpose(m_translation).m);

	//Camera translation
	m_cam_translation = T(-(int)m_camera_x, -(int)m_camera_y, 0);
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("cam_translation"), 1, GL_FALSE, Transpose(m_cam_translation).m);

	//Rotation
	mat4 rotation = ArbRotate(vec3(0, 0, 1), rot);
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("rotation"), 1, GL_FALSE, Transpose(rotation).m);

	//Scale
	m_scale = S((int)width, (int)height, 0);
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("scale"), 1, GL_FALSE, m_scale.m);

	//Projection
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("projection"), 1, GL_FALSE, Transpose(m_projection).m);

	//Render
	glBindVertexArray(m_texture_vao_ID);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_shader_program_texture->unuse();

	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
}

void Graphics::render_texture_ui(TextureType texture, float x, float y, float width, float height, float rot)
{
	m_shader_program_texture->use();

	GLint textureUniform;
	textureUniform = glGetUniformLocation(m_shader_program_texture->getProgramID(), "texture_sampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_id_array[texture]);

	//Translation
	m_translation = T((int)x, (int)y, 0);
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("translation"), 1, GL_FALSE, Transpose(m_translation).m);

	//Camera translation
	m_cam_translation = T(0, 0, 0);
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("cam_translation"), 1, GL_FALSE, Transpose(m_cam_translation).m);

	//Rotation
	mat4 rotation = ArbRotate(vec3(0, 0, 1), rot);
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("rotation"), 1, GL_FALSE, Transpose(rotation).m);

	//Scale
	m_scale = S(width, height, 0);
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("scale"), 1, GL_FALSE, m_scale.m);

	//Projection
	glUniformMatrix4fv(m_shader_program_texture->getUniformLocation("projection"), 1, GL_FALSE, Transpose(m_projection).m);

	//Render
	glBindVertexArray(m_texture_vao_ID);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_shader_program_texture->unuse();

	glActiveTexture(GL_TEXTURE0);
}

void Graphics::render_dynamic_text(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	m_shader_program_font->use();

	//Color
	glUniform3f(glGetUniformLocation(m_shader_program_font->getProgramID(), "text_color"), color.x, color.y, color.z);

	//Translation
	glUniformMatrix4fv(m_shader_program_font->getUniformLocation("projection"), 1, GL_FALSE, Transpose(m_projection).m);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_font_vao_ID);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Font_Character ch = m_characters[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character

		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};

		GLint textureUniform;
		textureUniform = glGetUniformLocation(m_shader_program_font->getProgramID(), "text");
		glUniform1i(textureUniform, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		// Render glyph texture over quad
		//glBindTexture(GL_TEXTURE_2D, ch.textureID);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_font_vbo_ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::render_particles(int number_of_particles, int particle_system_id, vec3 color)
{
	if (particle_system_id != -1)
	{
		m_shader_program_particle->use();

		//Camera translation
		m_cam_translation = T(-(int)m_camera_x, -(int)m_camera_y, 0);
		glUniformMatrix4fv(m_shader_program_particle->getUniformLocation("cam_translation"), 1, GL_FALSE, Transpose(m_cam_translation).m);

		//Scale
		m_scale = S(4, 4, 0);
		glUniformMatrix4fv(m_shader_program_particle->getUniformLocation("scale"), 1, GL_FALSE, Transpose(m_scale).m);

		//Color
		GLfloat m_color[3] = { color.x, color.y, color.z };
		glUniform3fv(m_shader_program_particle->getUniformLocation("color"), 1, m_color);

		//Projection
		glUniformMatrix4fv(m_shader_program_particle->getUniformLocation("projection"), 1, GL_FALSE, Transpose(m_projection).m);

		glBindVertexArray(m_particle_vao_ID);
		
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, m_particle_position_buffer_ID);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(particle_system_id * m_max_particles_per_system * 3 * sizeof(GLfloat)));

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, number_of_particles);

		m_shader_program_particle->unuse();

		glActiveTexture(GL_TEXTURE0);
	}
}

void Graphics::update_particles(vec3* particle_positions, int number_of_particles, int particle_system_id)
{
	if (particle_system_id != -1)
	{
		glBindVertexArray(m_particle_vao_ID);

		glBindBuffer(GL_ARRAY_BUFFER, m_particle_position_buffer_ID);
		glBufferSubData(GL_ARRAY_BUFFER, particle_system_id * m_max_particles_per_system * 3 * sizeof(GLfloat), number_of_particles * 3 * sizeof(GLfloat), particle_positions);
	}
}

void Graphics::swapWindow()
{
	SDL_GL_SwapWindow(_window);
}

void Graphics::setCameraPosition(float x, float y)
{
	m_camera_x = x;
	m_camera_y = y;
}

vec2 Graphics::getCameraPosition()
{
	return vec2{ (float)m_camera_x, (float)m_camera_y };
}

void Graphics::load_texture(int texture_index, int texture_width, int texture_height, int clip_x, int clip_y, int clip_width, int clip_height)
{
	//ROCKET
	SDL_Surface* surface = SDL_CreateRGBSurface(0, texture_width, texture_height, 32, 0, 0, 0, 0);
	blitSurface(m_sprite_sheet, surface, clip_x, clip_y, clip_width, clip_height);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);

	m_texture_id_array[texture_index] = texture_id;
}

void Graphics::updateGameLevelMaskTextureInOpenGL()
{
	//TODO do this more efficient, dont update the whole texture, only part that is changed.
	if (m_game_level->hasChanged()) {

		Rect change = m_game_level->getChange();

		glBindTexture(GL_TEXTURE_2D, m_game_level_mask_texture_id);

		glTexSubImage2D(GL_TEXTURE_2D, 0, change.x, change.y, change.width, change.height, GL_RED, GL_UNSIGNED_BYTE, (void*)m_game_level->getGameLevelData());
	}
}

vec2 Graphics::getOpenGLCoordinates(float x, float y)
{
	return vec2((x / m_window_width) * 2.0 * GRAPHIC_SCALE, (y / m_window_height) * 2.0 * GRAPHIC_SCALE);
}

void Graphics::load_sprite_sheet()
{
	m_sprite_sheet = IMG_Load("res/textures/sprite_sheet.png");
	if (m_sprite_sheet == NULL)
	{ 
		std::string s = "Error: ";
		s = SDL_GetError();
		error_output(s);
	}
}

void Graphics::load_textures_types()
{
	load_texture(TextureType::NONE, 40, 40, 400, 0, 40, 40);
	load_texture(TextureType::BULLET_10X10_1, 10, 10, 180, 0, 10, 10);
	load_texture(TextureType::BULLET_10X10_2, 10, 10, 190, 0, 10, 10);
	load_texture(TextureType::BULLET_10X10_3, 10, 10, 200, 0, 10, 10);
	load_texture(TextureType::BULLET_10X10_4, 10, 10, 210, 0, 10, 10);
	load_texture(TextureType::BULLET_10X10_5, 10, 10, 220, 0, 10, 10);

	load_texture(TextureType::GRANADE_10X10_1, 10, 10, 180, 10, 10, 10);
	load_texture(TextureType::GRANADE_10X10_2, 10, 10, 190, 10, 10, 10);

	load_texture(TextureType::EXPLOSION_40X40_1, 40, 40, 400, 0, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_2, 40, 40, 400, 0, 40, 40);

	load_texture(TextureType::EXPLOSION_40X40_3_FRAME1, 40, 40, 400, 40, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_3_FRAME2, 40, 40, 440, 40, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_3_FRAME3, 40, 40, 480, 40, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_3_FRAME4, 40, 40, 520, 40, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_3_FRAME5, 40, 40, 560, 40, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_3_FRAME6, 40, 40, 600, 40, 40, 40);

	load_texture(TextureType::EXPLOSION_40X40_4_FRAME1, 40, 40, 400, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME2, 40, 40, 440, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME3, 40, 40, 480, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME4, 40, 40, 520, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME5, 40, 40, 560, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME6, 40, 40, 600, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME7, 40, 40, 640, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME8, 40, 40, 680, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME9, 40, 40, 720, 80, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME10, 40, 40, 400, 120, 40, 40);
	load_texture(TextureType::EXPLOSION_40X40_4_FRAME11, 40, 40, 440, 120, 40, 40);

	load_texture(TextureType::GAME_LEVEL_TEXTURE_DIRT, 80, 80, 0, 400, 80, 80);
	load_texture(TextureType::GAME_LEVEL_TEXTURE_DIRT_RED, 80, 80, 80, 400, 80, 80);
	load_texture(TextureType::GAME_LEVEL_TEXTURE_DIRT_GREY, 80, 80, 160, 400, 80, 80);
	load_texture(TextureType::GAME_LEVEL_TEXTURE_SNOW, 80, 80, 240, 400, 80, 80);
	load_texture(TextureType::GAME_LEVEL_TEXTURE_SAND, 80, 80, 320, 400, 80, 80);
	load_texture(TextureType::GAME_LEVEL_TEXTURE_STONE, 80, 80, 0, 480, 80, 80);

	load_texture(TextureType::CHARACTER_SOLDIER_RIGHT_FRAME1, 20, 32, 0, 0, 20, 32);
	load_texture(TextureType::CHARACTER_SOLDIER_RIGHT_FRAME2, 20, 32, 20, 0, 20, 32);
	load_texture(TextureType::CHARACTER_SOLDIER_RIGHT_FRAME3, 20, 32, 40, 0, 20, 32);
	load_texture(TextureType::CHARACTER_SOLDIER_RIGHT_FRAME4, 20, 32, 60, 0, 20, 32);
	load_texture(TextureType::CHARACTER_SOLDIER_LEFT_FRAME1, 20, 32, 80, 0, 20, 32);
	load_texture(TextureType::CHARACTER_SOLDIER_LEFT_FRAME2, 20, 32, 100, 0, 20, 32);
	load_texture(TextureType::CHARACTER_SOLDIER_LEFT_FRAME3, 20, 32, 120, 0, 20, 32);
	load_texture(TextureType::CHARACTER_SOLDIER_LEFT_FRAME4, 20, 32, 140, 0, 20, 32);

	load_texture(TextureType::AIM_1, 12, 12, 260, 70, 12, 12);

	load_texture(TextureType::STANDARD_RIGHT_WEAPON, 40, 40, 180, 30, 40, 40);
	load_texture(TextureType::STANDARD_LEFT_WEAPON, 40, 40, 220, 30, 40, 40);
	load_texture(TextureType::STANDARD_MUZZLE_FLASH, 16, 16, 260, 30, 16, 16);

	load_texture(TextureType::SHOVEL_RIGHT, 40, 40, 180, 70, 40, 40);
	load_texture(TextureType::SHOVEL_LEFT, 40, 40, 220, 70, 40, 40);

	load_texture(TextureType::GRENADE_LAUNCHER_RIGHT_WEAPON, 40, 40, 180, 110, 40, 40);
	load_texture(TextureType::GRENADE_LAUNCHER_LEFT_WEAPON, 40, 40, 220, 110, 40, 40);

	load_texture(TextureType::HP_BAR, 40, 40, 180, 350, 40, 40);

	load_texture(TextureType::SHOVEL_ICON, 40, 40, 180, 270, 40, 40);
	load_texture(TextureType::SOLDIER_ROCKET_LAUNCHER_ICON, 40, 40, 220, 270, 40, 40);
	load_texture(TextureType::SOLDIER_RIFLE_ICON, 40, 40, 260, 270, 40, 40);

	load_texture(TextureType::BUTTON_1_128X64_IDLE, 128, 64, 400, 464, 128, 64);
	load_texture(TextureType::BUTTON_1_128X64_HIGHLIGHTED, 128, 64, 592, 464, 128, 64);

	load_texture(TextureType::RADIO_BUTTON_1_32X32_NOT_SELECTED, 32, 32, 400, 592, 32, 32);
	load_texture(TextureType::RADIO_BUTTON_1_32X32_SELECTED, 32, 32, 432, 592, 32, 32);
		
}

void Graphics::blitSurface(SDL_Surface* source, SDL_Surface* destination, int clip_x, int clip_y, int clip_width, int clip_height)
{
	SDL_Rect srcrect;
	srcrect.x = clip_x;
	srcrect.y = clip_y;
	srcrect.w = clip_width;
	srcrect.h = clip_height;

	//Blit
	SDL_BlitSurface(source, &srcrect, destination, NULL);
}

int Graphics::getParticleSystemID()
{

	for (int i = 0; i < m_number_of_particle_system_ids; i++)
	{
		if (m_occupied_particle_system_ID[i] == false)
		{
			m_occupied_particle_system_ID[i] = true;

			return i;
		}
	}

	return -1;
}

void Graphics::returnParticleSystemID(int id)
{
	if (id >= 0 && id < m_number_of_particle_system_ids)
	{
		
		m_occupied_particle_system_ID[id] = false;
	}
}

void Graphics::init_font_graphics()
{
	//INIT FREETYPE
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "res/fonts/OpenSans-Bold.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Font_Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		m_characters.insert(std::pair<GLchar, Font_Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &m_font_vao_ID);
	glGenBuffers(1, &m_font_vbo_ID);
	glBindVertexArray(m_font_vao_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_font_vbo_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}