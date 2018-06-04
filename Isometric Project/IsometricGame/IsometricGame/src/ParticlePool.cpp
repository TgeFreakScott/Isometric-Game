/// <summary>
/// @authors: Emmett Byrne - C00220950
///			  Przemek Tomczyk - C00218004
///			  Patryk Wysocki - C00213855
///			  Savannah Bolger - C00215747
///			  David Scott - C00208031
/// @date started: 5/03/2018
/// @date finished: 23/03/2018
/// 
/// </summary>

#include "ParticlePool.h"


void ParticlePool::create(sf::Texture & t_particleTexture, sf::Vector2f &  t_particlePosition)
{
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}

	m_particles.at(m_nextAvailable).createParticle(t_particleTexture, t_particlePosition);
}

void ParticlePool::createMissileParticle(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition)
{
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}

	m_particles.at(m_nextAvailable).createEnemyParticle(t_particleTexture, t_particlePosition);
}

void ParticlePool::update(sf::Time & t_time)
{

	int activeCount = 0;

	m_poolFull = false;
	for (int index = 0; index < s_POOL_SIZE; index++)
	{
		if (m_particles.at(index).update(t_time))
		{
			m_nextAvailable = index;
		}
		else
		{
			activeCount++;
		}
	}

	if (s_POOL_SIZE == activeCount)
	{
		m_poolFull = true;
	}
}

void ParticlePool::render(sf::RenderWindow & t_window)
{
	for (int index = 0; index < s_POOL_SIZE; index++)
	{
		if (m_particles.at(index).particleActive())
		{
			m_particles.at(index).render(t_window);
		}
	}
}
