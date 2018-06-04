
// @authors: David Scott - C00208031
//           Nikita Mullins - C00
// @date started: 
// @date finished: 


#include "Game.h"
#include <iostream>
#include "Thor\Math.hpp"

unsigned int height = 1080;
unsigned int width = 1920;

Game::Game() :
	m_window{ sf::VideoMode{ width, height, 32 },"Isometric", sf::Style::Default },
	m_exitGame{ false } //when true game will exit
{
	m_window.setVerticalSyncEnabled(true);

	m_backgroundSprite.setTexture(m_resourceMng.getTexture(TextureID::BACKGROUND));
	m_backgroundSprite.setRotation(45);
	m_backgroundSprite.setOrigin(m_backgroundSprite.getLocalBounds().width / 2, m_backgroundSprite.getLocalBounds().height / 2);
	m_backgroundSprite.setScale(2.f, 2.f);
	m_backgroundSprite.setPosition(width / 2, height / 2);

	m_circle.setRadius(30);
	m_circle.setOrigin(15, 15);
	m_circle.setPosition(width / 2, height / 2);
	m_circle.setFillColor(sf::Color::Yellow);

}


Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == event.type) //user key press
		{
			if (sf::Keyboard::Escape == event.key.code)
			{
				m_exitGame = true;
			}
		}
	}
}

void Game::update(sf::Time t_deltaTime)
{
	sf::Event event;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_circle.setPosition(m_circle.getPosition().x + 10, m_circle.getPosition().y - 10);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_circle.setPosition(m_circle.getPosition().x - 10, m_circle.getPosition().y + 10);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_circle.setPosition(m_circle.getPosition().x + 10, m_circle.getPosition().y + 10);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_circle.setPosition(m_circle.getPosition().x - 10, m_circle.getPosition().y - 10);
	}
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	sf::View v = m_window.getDefaultView();

	v.setSize(v.getSize().x, v.getSize().y * 2);
	//v.setCenter(v.getSize() *.5f);
	v.setCenter(m_circle.getPosition());
	m_window.setView(v);
	m_window.draw(m_backgroundSprite);
	m_window.draw(m_circle);

	m_window.setView(m_window.getDefaultView());


	m_window.display();
}
