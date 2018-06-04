
#ifndef GAME
#define GAME


#include <SFML/Graphics.hpp>
#include "sfeMovie\Movie.hpp"
#include "sfeMovie\StreamSelection.hpp"
#include "sfeMovie\Visibility.hpp"
#include "ResourceManager.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	ResourceManager m_resourceMng;

	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	sf::RenderWindow m_window;

	bool m_exitGame;

	sf::Texture m_backgroundTexture; // texture used for background
	sf::Sprite m_backgroundSprite; // texture used for background

	sf::CircleShape m_circle;
	//Xbox360Controller m_controller;

	//Variables for SFE Movie	
	//sfe::Movie m_intro;

};

#endif // !GAME

