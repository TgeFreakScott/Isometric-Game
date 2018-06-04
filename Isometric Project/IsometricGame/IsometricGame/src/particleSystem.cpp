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

#include "particleSystem.h"

/// <summary>
/// @brief render function to draw the particles on the screen
/// </summary>
/// <param name="t_window"></param>

void ParticleSystem::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_system);
}

/// <summary>
/// @brief update function for the particles 
/// updates the particles system and checks to see if the system has finished
/// return  true if finsihed else false
/// </summary>
/// <param name="t_deltaTime"></param>
/// <returns></returns>

bool ParticleSystem::update(sf::Time t_deltaTime)
{
	m_system.update(t_deltaTime);	
	if (t_deltaTime.asSeconds() > PARTICLE_TIME)
	{
		systemActive = false;
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// @brief creates particle system
/// </summary>
/// <param name="t_particleTexture">texture used</param>
/// <param name="t_particleLifeTime"> how long the particles last for , last for a value between the two values</param>
/// <param name="t_particlePosition"> position of the particles on the screen , need to add the offset value in level.cpp to position it correctly</param>
/// <param name="t_particleVelocity"> determines how far out the particles travel </param>
/// <param name="t_particleRect"> used only when you have multiple images in one texture - input empty vector if none</param>

void ParticleSystem::createParticle(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition)
{
	systemActive = true;		// system is active

	// clears all values from the system 
	m_system.clearAffectors();
	m_system.clearEmitters();
	m_system.clearParticles();


	m_system.setTexture(t_particleTexture);

	thor::UniversalEmitter emitter;		// emitter for the system - what causes stuff to happen in the particle system

	// used only if there are int rects used
	
	// how quickly particles/ how many particles spawn 
	emitter.setEmissionRate(20);	

	// sets the lifetime of individual particles to be between two values inputted
	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.1), sf::seconds(0.3)));

	// sets position
	emitter.setParticlePosition(thor::Distributions::circle(t_particlePosition, 5));	

	// velocity of the particles , 360 means they will all travel in any direction around point
	emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(100.0,0.0f),  360.0));	

	// rotates the individual particles - not notible if only using circle texture - can be commented out / gotten rid of if only circle textures used
	emitter.setParticleRotation(thor::Distributions::uniform(0.0f, 360.0f));	

	// adds emitter to system and sets how long emitter lasts
	m_system.addEmitter(emitter, sf::seconds(PARTICLE_TIME));
}

void ParticleSystem::createEnemyParticle(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition)
{
	systemActive = true;		// system is active

								// clears all values from the system 
	m_system.clearAffectors();
	m_system.clearEmitters();
	m_system.clearParticles();


	m_system.setTexture(t_particleTexture);

	sf::IntRect rect0(0, 0, 8, 8);
	sf::IntRect rect1(8, 0, 8, 8);

	unsigned int index0 = m_system.addTextureRect(rect0);
	unsigned int index1 = m_system.addTextureRect(rect1);



	thor::UniversalEmitter emitter;		// emitter for the system - what causes stuff to happen in the particle system

										// used only if there are int rects used

	emitter.setParticleTextureIndex(thor::Distributions::uniform(0, 1));
										// how quickly particles/ how many particles spawn 
	emitter.setEmissionRate(600);

	// sets the lifetime of individual particles to be between two values inputted
	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.1), sf::seconds(0.5)));

	// sets position
	emitter.setParticlePosition(thor::Distributions::circle(t_particlePosition, 5));

	// velocity of the particles , 360 means they will all travel in any direction around point
	emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(100.0, 0.0f), 360.0));

	// rotates the individual particles - not notible if only using circle texture - can be commented out / gotten rid of if only circle textures used
	emitter.setParticleRotation(thor::Distributions::uniform(0.0f, 360.0f));

	// adds emitter to system and sets how long emitter lasts
	m_system.addEmitter(emitter, sf::seconds(.05));
}

/// <summary>
/// @brief returns the current state of the system active bool
/// </summary>
/// <returns></returns>

bool ParticleSystem::particleActive()
{
	return systemActive;
}
