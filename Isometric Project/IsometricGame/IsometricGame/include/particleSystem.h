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

#include "Thor\Particles.hpp"
#include "Thor\Math.hpp"
#include <SFML\Graphics.hpp>

class ParticleSystem
{
public:
	ParticleSystem() = default;

	void render(sf::RenderWindow & t_window);
	bool update(sf::Time t_deltaTime);

	void createParticle(sf::Texture & t_particleTexture,sf::Vector2f & t_particlePosition);
	void createEnemyParticle(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition);
	bool particleActive();


private:
	// particle system used
	thor::ParticleSystem m_system;

	// bool if the system is currently activate
	bool systemActive{ false };


	// how long the emitters last for
	const  double PARTICLE_TIME = 0.1;

	
};

