#pragma once

#include "positional_game_entity.h"
#include "math_utils.h"
#include "timer.h"

#include <random>
#include <vector>

struct Particle 
{
	float scale;

	float x_vel;
	float y_vel;

	int life_time;
};

class Particle_System : public Positional_Game_Entity
{
public:
	Particle_System(float x, float y, int particle, vec3 color, int life_time, float gravity);
	Particle_System(float x, float y, std::vector<vec2> particle_positions, vec3 color, int life_time, float gravity);
	~Particle_System();

	void on_tick(unsigned int delta_time);
	void draw();

	//FUNCTIONS TO CONSTRUCT PARTICLE SYSTEM
	void set_gravity(float gravity);

private:
	//VARIABLES
	std::random_device m_random;

	vec3 m_color;

	int m_life_time;

	int m_number_of_particles;

	float m_gravity;

	//OTHER
	Timer m_update_timer;
	int m_particle_system_id;
	vec3* m_positions;
	Particle* m_particles;
};

