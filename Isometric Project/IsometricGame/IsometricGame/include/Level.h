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

#ifndef LEVEL
#define LEVEL

#include <SFML/Graphics.hpp>
#include "Utility.h"
#include "Player.h"
#include "XBOX360CONTROLLER.h"
#include "ResourceManager.h"
#include "Flare.h"
#include "Missile.h"
#include "HUD.h"
#include "LevelLoader.h"
//#include "landingPad.h"

class Level
{
public:
	Level(ResourceManager & t_manager, Upgrades & t_upgrade, MenuStates & state, sf::RenderWindow & t_window);
	~Level();

	void init();
	void processEvents(Xbox360Controller & t_pad);
	void update(sf::Time t_deltaTime);
	void render(sf::RenderWindow & t_window);
	void setupLevel(int levelId);
	void updateOpening(sf::Time t_deltaTime);
	void updateGameplay(sf::Time t_deltaTime);
	void reset();
	bool getChangeMenu();
	void resetChangeMenu();

private:

	enum class LevelState
	{
		OpeningScene,
		Gameplay,
		Win,
		Loss
	} m_currentState;

	void collisionHandler();
	void nextStage();
	void setPadsUp();
	void handleAnim(sf::Time t_deltaTime);

	static int const MAX_POLY{ 7 };
	const float OPENING_DURATION_1{ 2 };
	const float OPENING_DURATION_2{ 2 };
	const float OPENING_DURATION_3{ 1 };
	const float OPENING_DURATION_4{ 1 };
	const float ZOOMED_IN{ 0.5f };
	const float ZOOMED_OUT{ 0.75f };
	
	MenuStates & m_menuState;
	ResourceManager & m_resourceMng;
	Player m_player;
	Missile m_missile;
	HUD m_hud;
	//LandingPad m_landingPad;
	//LandingPad m_endPlatform;
	static const int S_LEVEL_TILES{ 4 };
	sf::Sprite m_terrain[S_LEVEL_TILES]; // sprite that is used as the terrain
	sf::View m_view;

	bool m_rockExplosion = false;


	//smoke  shader test.
	float m_totalTime = 0;

	//opening data
	float m_openingTimer;
	int m_openingStage;

	std::vector<c2Circle> m_rockBounds;
	std::vector<sf::Sprite> m_rockSprites;

	sf::Sprite m_explosionSprite;
	Timer m_frameHandler;
	int m_frameID{ 0 };
	bool m_shown{ false };

	sf::Vector2f m_velocity;
	// Wall sprites
	std::vector<sf::Sprite> m_wallSprites;

	sf::Sprite m_backgroundSprite;
	sf::Shader m_backgroundShader;
	LevelData m_level;
	int m_currentLevel;

	Tinyc2Debug m_tinyDebug;
	std::vector<c2Poly> m_terrainVector;
	c2Poly m_bigTerrainBox;

	c2Raycast testRay;

	//Text object for winning and losing
	sf::Text m_winText;
	sf::Text m_loseText;

	bool m_changeMenu{ false };
};

#endif // !LEVEL