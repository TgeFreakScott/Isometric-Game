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

#pragma once


#include <SFML\Graphics.hpp>
#include <array>
#include "particleSystem.h"

/// <summary>
/// @brief  basically it's the same as a projectile pool but it has particles instead
/// </summary>
class ParticlePool
{
public:
	ParticlePool() = default;


	void create(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition);
	void createMissileParticle(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition);
	void update(sf::Time & t_time);
	void render(sf::RenderWindow & t_window);
private:

	static const int s_POOL_SIZE = 1000;

	std::array<ParticleSystem, s_POOL_SIZE> m_particles;

	int m_nextAvailable{ 0 };

	bool m_poolFull{ false };

	
};

