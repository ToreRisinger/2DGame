#include "particle_system.h"

#include "graphics.h"
#include "game.h"

Particle_System::Particle_System(float x, float y, int particles, vec3 color, int life_time, float gravity) : Positional_Game_Entity{ x, y, 0, 0}
{
	m_gravity = gravity;
	m_life_time = life_time;
	m_number_of_particles = particles;
	m_color = color;

	m_particle_system_id = Game::graphics->getParticleSystemID();

	if (m_particle_system_id != -1)
	{
		m_particles = new Particle[m_number_of_particles];
		m_positions = new vec3[m_number_of_particles];

		float rand;
		for (int i= 0; i < m_number_of_particles; i++)
		{
			m_positions[i] = vec3{ x, y, 0.0f };

			vec2 coord = vec2{ (float)((int)(m_random() % 30) - 15) / 200, ((float)((int)(m_random() % 30) - 15) / 200) };
			m_particles[i].x_vel = coord.x;
			m_particles[i].y_vel = coord.y;

		}

		m_update_timer.start();
		Game::graphics->update_particles(m_positions, m_number_of_particles, m_particle_system_id);
	}
	else
	{
		
		m_is_dead = true;
	}
}

Particle_System::Particle_System(float x, float y, std::vector<vec2> particle_positions, vec3 color, int life_time, float gravity) : Positional_Game_Entity{ x, y, 0, 0 }
{
	m_gravity = gravity;
	m_life_time = life_time;
	
	int total_particles = particle_positions.size();

	int max_number_of_particles = 5;

	float chance_of_spawning_particle = (float)max_number_of_particles / (float)total_particles;
	
	m_color = color;

	m_particle_system_id = Game::graphics->getParticleSystemID();

	if (m_particle_system_id != -1)
	{
		m_particles = new Particle[total_particles];
		m_positions = new vec3[total_particles];

		int counter = 0;
		int number_of_particles_spawned = 0;
		while (number_of_particles_spawned < max_number_of_particles && counter < total_particles)
		{	
			if ((float)((m_random() % 100) + 1) < chance_of_spawning_particle * 100.0)
			{
				m_positions[number_of_particles_spawned] = vec3{ particle_positions.at(counter).x, particle_positions.at(counter).y, 0.0f };
				vec2 coord = vec2{ (float)((int)(m_random() % 30) - 15) / 300, ((float)((int)(m_random() % 30) - 15) / 300) };
				m_particles[number_of_particles_spawned].x_vel = coord.x;
				m_particles[number_of_particles_spawned].y_vel = coord.y;
				number_of_particles_spawned++;
			}
			counter++;
		}

		m_number_of_particles = number_of_particles_spawned;

		m_update_timer.start();
		Game::graphics->update_particles(m_positions, m_number_of_particles, m_particle_system_id);
	}
	else
	{

		m_is_dead = true;
	}
}

Particle_System::~Particle_System()
{
	delete[] m_particles;
	delete[] m_positions;
	Game::graphics->returnParticleSystemID(m_particle_system_id);
}

void Particle_System::on_tick(unsigned int delta_time)
{
	m_life_time -= delta_time;

	if (m_life_time <= 0) 
	{
		m_is_dead = true;
	}

	for (int i = 0; i < m_number_of_particles; i++)
	{
		m_particles[i].y_vel -= m_gravity;

		m_positions[i].x = m_positions[i].x + m_particles[i].x_vel * delta_time;
		m_positions[i].y = m_positions[i].y + m_particles[i].y_vel * delta_time;
	}
	
}

void Particle_System::draw()
{
	if (m_update_timer.getTime() >= 16)
	{
		m_update_timer.start();
		Game::graphics->update_particles(m_positions, m_number_of_particles, m_particle_system_id);
	}

	Game::graphics->render_particles(m_number_of_particles, m_particle_system_id, m_color);
}

void Particle_System::set_gravity(float gravity)
{
	m_gravity = (Game::graphics->getOpenGLCoordinates(gravity, 0)).x;
}
