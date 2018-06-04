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

#include "Level.h"
#define COLOR_NORMAL sf::Color(41, 128, 185)
#define COLOR_HIT sf::Color(231, 76, 60)

Level::Level(ResourceManager & t_manager, Upgrades & t_upgrade, MenuStates & state, sf::RenderWindow & t_window) :
	m_resourceMng(t_manager),
	m_player(t_manager, t_upgrade, t_window),
	m_missile(t_manager),
	m_hud(t_manager, m_player),
	m_tinyDebug(t_window),
	m_landingPad(t_manager),
	m_endPlatform(t_manager),
	m_openingTimer(0),
	m_openingStage(1),
	m_currentState(LevelState::Gameplay),
	m_velocity{0,0},
	m_menuState(state)
{
	//setupLevel(2);

	m_frameHandler.duration = sf::milliseconds(35);
	sf::Texture & blankTexture = m_resourceMng.getTexture(TextureID::BLANK);
	m_explosionSprite.setTexture(m_resourceMng.getTexture(TextureID::EXPLOSION));

	if (!m_backgroundShader.loadFromFile(("ASSETS/SHADERS/background.Frag"), sf::Shader::Fragment))
	{
		std::cout << "error with loading a shader" << std::endl;
	}
	m_backgroundSprite.setTexture(blankTexture);
	m_backgroundShader.setUniform("time", 2.0f);
	//1920,1080
	m_backgroundShader.setUniform("resolution", sf::Vector2f(1920,1080));
	m_backgroundSprite.setPosition(0.0f, 0.0f);

	m_view.setSize(Utility::screenWidth * ZOOMED_IN, Utility::screenHeight * ZOOMED_IN);
	
	m_loseText.setFont(m_resourceMng.getFont(FontID::ARIBLK));
	m_loseText.setFillColor(sf::Color::Red);
	m_loseText.setString("You Lose \n Press Start to reset \n Bress back to return to menu");
	m_loseText.setPosition(sf::VideoMode::getDesktopMode().width / 10, sf::VideoMode::getDesktopMode().height / 2);
	m_winText.setFont(m_resourceMng.getFont(FontID::ARIBLK));
	m_winText.setFillColor(sf::Color::Red);
	m_winText.setString("You Win \n Press Start to continue to next level \n Bress back to return to menu");
	m_winText.setPosition(sf::VideoMode::getDesktopMode().width / 10, sf::VideoMode::getDesktopMode().height / 2);
}

Level::~Level()
{
}

void Level::init()
{
	
}

void Level::processEvents(Xbox360Controller & t_pad)
{
	if (m_currentState == LevelState::Gameplay)
	{
		m_player.processEvents(t_pad);
	}
	if (m_currentState == LevelState::Loss)
	{
		if (t_pad.m_currentState.Start)
		{
			m_player.init(m_level.m_player.m_position);
			m_currentState = LevelState::Gameplay;
		}
		if (t_pad.m_currentState.Back)
		{
			m_menuState = MenuStates::StateMainMenu;
			m_changeMenu = true;
		}
	}
	if (m_currentState == LevelState::Win)
	{
		if (t_pad.m_currentState.Start)
		{
			setupLevel(m_currentLevel + 1);
		}
	}
}

void Level::update(sf::Time t_deltaTime)
{
	switch (m_currentState)
	{
	case LevelState::OpeningScene:
		updateOpening(t_deltaTime);
		break;
	case LevelState::Gameplay:
		updateGameplay(t_deltaTime);
		handleAnim(t_deltaTime);
		break;
	case LevelState::Win:
		break;
	case LevelState::Loss:
		break;
	default:
		break;
	}
}

void Level::render(sf::RenderWindow & t_window)
{
	sf::IntRect temp_windowRect;

	t_window.setView(m_view);
	// set the intRect that will render the level sprites that are currently in the view
	// set the top left point
	{
		temp_windowRect.left = (m_view.getCenter().x - m_view.getSize().x / 2);
		temp_windowRect.top = (m_view.getCenter().y - m_view.getSize().y / 2);
	}
	// set the size of the box
	t_window.draw(m_backgroundSprite, &m_backgroundShader);
	{
		temp_windowRect.width = (m_view.getSize().x);
		temp_windowRect.height = (m_view.getSize().y);
	}

	for (int index = 0; index < S_LEVEL_TILES; index++)
	{
		if (temp_windowRect.intersects(m_terrain[index].getTextureRect()))
		{
			// true then draw the tile
			t_window.draw(m_terrain[index]);
		}
	}

	m_missile.render(t_window);

	if (!m_rockExplosion)
	{
		for (auto &sprite : m_rockSprites)
		{
			t_window.draw(sprite);
		}
	}

	m_landingPad.render(t_window);
	m_endPlatform.render(t_window);

	if (m_rockExplosion)
	{
		t_window.draw(m_explosionSprite);
	}

	m_player.render(t_window);

	for (int i = 0; i < m_terrainVector.size(); i++)
	{
		
		m_tinyDebug.draw(m_terrainVector[i], COLOR_NORMAL);
	}

	m_tinyDebug.draw(m_bigTerrainBox, COLOR_NORMAL);

	for (int index = 0; index < m_rockBounds.size(); index++)
	{
		m_tinyDebug.draw(m_rockBounds[index], COLOR_NORMAL);
	}
	sf::View tempView;

	t_window.setView(tempView);

	m_hud.render(t_window);

	if (m_currentState == LevelState::Loss)
	{
		t_window.draw(m_loseText);
	}
	if (m_currentState == LevelState::Win)
	{
		t_window.draw(m_winText);
	}

}

void Level::setupLevel(int levelId)
{
	m_terrainVector.clear();

	m_currentLevel = levelId;

	m_currentState = LevelState::OpeningScene;

	// load level data from a yaml file
	if (!LevelLoader::load(levelId, m_level))
	{
		return;
	}

	m_player.init(m_level.m_player.m_position);
	m_landingPad.init(m_level.m_platform[0].m_type, m_level.m_platform[0].m_position);
	m_endPlatform.init(m_level.m_platform[1].m_type, m_level.m_platform[1].m_position);
	m_missile.init(m_level.m_missiles[0].m_position);

	int temp_index = 0; // local index that is used to go through data in the level data

	int max_poly;
	if (levelId == 1)
	{
		max_poly = 9 + m_level.m_terrain.size() / 3;
	} 
	else
	{
		max_poly = 20 + m_level.m_terrain.size() / 3;
	}

	for (int i = 0; i < max_poly; i++)
	{
		c2Poly poly;
		poly.count = 3;

		poly.verts[0] = c2V(m_level.m_terrain[temp_index].m_position.x, m_level.m_terrain[temp_index].m_position.y);
		poly.verts[1] = c2V(m_level.m_terrain[temp_index + 1].m_position.x, m_level.m_terrain[temp_index + 1].m_position.y);
		poly.verts[2] = c2V(m_level.m_terrain[temp_index + 2].m_position.x, m_level.m_terrain[temp_index + 2].m_position.y);

		temp_index += 2;

		c2MakePoly(&poly);

		if (i == 24 && levelId == 1)
		{
			temp_index++;
		}

		if (i == 17 && levelId == 2)
		{
			temp_index++;
		}

		m_terrainVector.push_back(poly);
	}

	if (levelId == 1)
	{
   		temp_index = 68;
		m_bigTerrainBox.count = 5;
		m_bigTerrainBox.verts[0] = c2V(m_level.m_terrain[temp_index].m_position.x, m_level.m_terrain[temp_index].m_position.y);
		m_bigTerrainBox.verts[1] = c2V(m_level.m_terrain[temp_index + 1].m_position.x, m_level.m_terrain[temp_index + 1].m_position.y);
		m_bigTerrainBox.verts[2] = c2V(m_level.m_terrain[temp_index + 2].m_position.x, m_level.m_terrain[temp_index + 2].m_position.y);
		m_bigTerrainBox.verts[3] = c2V(m_level.m_terrain[temp_index + 3].m_position.x, m_level.m_terrain[temp_index + 3].m_position.y);
		m_bigTerrainBox.verts[4] = c2V(m_level.m_terrain[temp_index + 4].m_position.x, m_level.m_terrain[temp_index + 4].m_position.y);
		c2MakePoly(&m_bigTerrainBox);
	}
	else
	{
		m_bigTerrainBox.count = 5;
		m_bigTerrainBox.verts[0] = c2V(0, 0);
		m_bigTerrainBox.verts[1] = c2V(0, 0);
		m_bigTerrainBox.verts[2] = c2V(0, 0);
		m_bigTerrainBox.verts[3] = c2V(0, 0);
		m_bigTerrainBox.verts[4] = c2V(0, 0);
		c2MakePoly(&m_bigTerrainBox);

	}

	if (m_rockExplosion == false)
	{
		for (auto &obstacle : m_level.m_obstacles)
		{

			sf::Sprite rock;
			c2Circle circle;
			rock.setTexture(m_resourceMng.getTexture(TextureID::ROCK_1));
			rock.setScale(0.18, 0.18);
			rock.setOrigin(rock.getLocalBounds().width / 2, rock.getLocalBounds().height / 2);
			rock.setPosition(obstacle.m_position);
			rock.rotate(obstacle.m_rotation);
			m_explosionSprite.setPosition(obstacle.m_position);
			circle.p = c2V(obstacle.m_position.x, obstacle.m_position.y);
			circle.r = (rock.getGlobalBounds().width / 2) - 15;

			m_rockSprites.push_back(rock);
			m_rockBounds.push_back(circle);

		}
	}

	if (levelId == 1)
	{
		for (int index = 0; index < S_LEVEL_TILES; index++)
		{
			m_terrain[index].setTexture(m_resourceMng.getTexture(TextureID::TERRAIN));
			m_terrain[index].setTextureRect(sf::IntRect(0 + 480 * index, 0, 480, 1080));
			m_terrain[index].setPosition(0.0f + 480 * index, 0.0f);
		}
	}

	if (levelId == 2)
	{
		for (int index = 0; index < S_LEVEL_TILES; index++)
		{
			m_terrain[index].setTexture(m_resourceMng.getTexture(TextureID::TERRAIN2));
			m_terrain[index].setTextureRect(sf::IntRect(0 + 480 * index, 0, 480, 1080));
			m_terrain[index].setPosition(0.0f + 480 * index, 0.0f);
		}
	}
}

void Level::updateOpening(sf::Time t_deltaTime)
{
	m_landingPad.update();
	m_endPlatform.update();
	
	float scaler;
	float size;
	sf::Vector2f pos;
	m_openingTimer += t_deltaTime.asSeconds();

	switch (m_openingStage)
	{
	case 1:
		scaler = m_openingTimer / OPENING_DURATION_1;
		size = ZOOMED_IN + (ZOOMED_OUT - ZOOMED_IN) * scaler;
		m_view.setSize(Utility::screenWidth * size, Utility::screenHeight * size);

		m_view.setCenter(m_player.getPosition());

		if (m_openingTimer > OPENING_DURATION_1)
		{
			nextStage();
		}
		break;
	case 2:
		scaler = m_openingTimer / OPENING_DURATION_2;

		pos = m_player.getPosition() + (m_endPlatform.getPosition() - m_player.getPosition()) * scaler;

		m_view.setCenter(pos);

		if (m_openingTimer > OPENING_DURATION_2)
		{
			nextStage();
		}
		break;
	case 3:
		scaler = m_openingTimer / OPENING_DURATION_3;
		size = ZOOMED_OUT - (ZOOMED_OUT - ZOOMED_IN) * scaler;
		m_view.setSize(Utility::screenWidth * size, Utility::screenHeight * size);

		m_view.setCenter(m_endPlatform.getPosition());

		if (m_openingTimer > OPENING_DURATION_3)
		{
			nextStage();
		}
		break;
	case 4:
		scaler = m_openingTimer / OPENING_DURATION_4;

		pos = m_endPlatform.getPosition() - (m_endPlatform.getPosition() - m_player.getPosition()) * scaler;

		m_view.setCenter(pos);

		if (m_openingTimer > OPENING_DURATION_4)
		{
			nextStage();
		}
		break;
	default:
		break;
	}
}

void Level::updateGameplay(sf::Time t_deltaTime)
{
	m_totalTime += t_deltaTime.asSeconds();

	m_backgroundShader.setUniform("time", m_totalTime);
	m_hud.update(m_player.getTemperature(), m_player.getFuel(), t_deltaTime);
	m_player.update(t_deltaTime);
	m_landingPad.update();
	m_endPlatform.update();
	if (m_player.getPosition().x - m_view.getSize().x / 2.0f > 0.0f &&
		m_player.getPosition().x + m_view.getSize().x / 2.0f < Utility::screenWidth)
	{
		m_view.setCenter(m_player.getPosition());
	}
	else
	{
		if (m_player.getPosition().x - m_view.getSize().x / 2.0f < 0.0f)
		{
			m_view.setCenter(m_view.getSize().x / 2.0f, m_player.getPosition().y);
		}
		else
		{
			m_view.setCenter(Utility::screenWidth - m_view.getSize().x / 2.0f, m_player.getPosition().y);
		}
	}
	m_missile.update(t_deltaTime, m_player.getPosition(), m_player.getHottest());


	collisionHandler();
}

bool Level::getChangeMenu()
{
	return m_changeMenu;
}

void Level::resetChangeMenu()
{
	m_changeMenu = false;
}

void Level::collisionHandler()
{
	c2Raycast cast;
	for (int i = 0; i < m_terrainVector.size(); i++)
	{
		//player to terrain
		if (c2CircletoPoly(m_player.getBounds(), &m_terrainVector[i], NULL))
		{
			m_currentState = LevelState::Loss;
		}

		//flare to terrain
		if (c2CircletoPoly(m_player.getFlare().getBounds(), &m_terrainVector[i], NULL))
		{
			m_player.getFlare().setStick(true);
		}

		//missile raycast ot terrain
		if (c2RaytoPoly(m_missile.getRay(), &m_terrainVector[i], NULL, &cast))
		{
			m_missile.setAvoidVector(sf::Vector2f(cast.n.x, cast.n.y));
		}

		//right truster to terrain
		if (c2CircletoPoly(m_player.getThrusters(1), &m_terrainVector[i], NULL))
		{
			m_player.setDamagedLeft(true);
		}

		//left truster to terrain
		if (c2CircletoPoly(m_player.getThrusters(2), &m_terrainVector[i], NULL))
		{
			m_player.setDamagedRight(true);
		}

		//missile to terrain
		if (c2CircletoPoly(m_missile.getBounds(), &m_terrainVector[i], NULL))
		{
			m_missile.setAlive(false);

			if (c2CircletoPoly(m_player.getThrusters(2), &m_terrainVector[i], NULL))
			{
			}
		}

		//flare to rock
		for (int index = 0; index < m_rockBounds.size(); index++)
		{
			if (c2CircletoCircle(m_player.getFlare().getBounds(), m_rockBounds[0]))
			{
				m_player.getFlare().setStick(true);
			}
		}
	}

	// player to giant poly
	if (c2CircletoPoly(m_player.getBounds(), &m_bigTerrainBox, NULL))
	{
		m_currentState = LevelState::Loss;
	}

	//missile to player
	if (c2CircletoCircle(m_player.getBounds(), m_missile.getBounds()))
	{
		m_missile.setAlive(false);
		m_currentState = LevelState::Loss;
	}
	if (c2CircletoCircle(m_player.getThrusters(2), m_missile.getBounds()))
	{
		m_missile.setAlive(false);
		m_currentState = LevelState::Loss;
	}
	if (c2CircletoCircle(m_player.getThrusters(1), m_missile.getBounds()))
	{
		m_missile.setAlive(false);
		m_currentState = LevelState::Loss;
	}

	//player to landing pad
	if (c2CircletoAABB(m_player.getBounds(), m_endPlatform.getBounds()))
	{
		int test  = thor::length(m_player.getVelocity());
		if (m_player.getRotation() < 105.0f 
			&& m_player.getRotation() > 75.0f)
		{
			if (test < 25.0f)
			{
				m_currentState = LevelState::Win;
			}
			else
			{
				m_currentState = LevelState::Loss;
			}
		}
	}
	
	//playing to launch pad
	if (c2CircletoAABB(m_player.getBounds(), m_landingPad.getBounds()))
	{
		int test = thor::length(m_player.getVelocity());
		if (m_player.getRotation() < 105.0f
			&& m_player.getRotation() > 75.0f
			&& test < 25.0f)
		{
			m_player.setPosition(sf::Vector2f(m_player.getPosition().x, m_landingPad.getPosition().y - m_player.getBounds().r - m_landingPad.getHeight()));
			m_player.setVelocity(sf::Vector2f(0, 0));
		}
		else
		{
			m_currentState = LevelState::Loss;
		}
	}

	//Player to Rocks
	for (int index = 0; index < m_rockBounds.size(); index++)
	{
		if (c2CircletoCircle(m_player.getBounds(), m_rockBounds[0]))
		{
			m_explosionSprite.setOrigin(m_explosionSprite.getLocalBounds().width / 2, m_explosionSprite.getLocalBounds().height / 2);
			if (index == 0)
			{
				if (!m_rockExplosion)
				{
					m_rockExplosion = true;
					m_explosionSprite.setPosition(680, 540);
					m_currentState = LevelState::Loss;
				}
			}
			m_rockExplosion = true;
		}
	
	}

	//Missile to Rocks
	for (int index = 0; index < m_rockBounds.size(); index++)
	{
		if (c2CircletoCircle(m_missile.getBounds(), m_rockBounds[index]))
		{
			m_explosionSprite.setOrigin(m_explosionSprite.getLocalBounds().width / 2, m_explosionSprite.getLocalBounds().height / 2);
			if (index == 0)
			{
				if (!m_rockExplosion)
				{
					m_rockExplosion = true;
					m_explosionSprite.setPosition(640, 540);
				}
			}
			if (index == 1)
			{
				if (!m_rockExplosion)
				{
					m_rockExplosion = true;
					m_explosionSprite.setPosition(660, 520);
				}
			}
			if (index == 2)
			{
				if (!m_rockExplosion)
				{
					m_rockExplosion = true;
					m_explosionSprite.setPosition(680, 500);
				}
			}
			if (index == 3)
			{
				if (!m_rockExplosion)
				{
					m_rockExplosion = true;
					m_explosionSprite.setPosition(700, 480);
				}
			}
			m_rockExplosion = true;
		}

	}

}

void Level::nextStage()
{
	m_openingStage++;
	m_openingTimer = 0;

	if (m_openingStage > 4)
	{
		m_currentState = LevelState::Gameplay;
		m_openingStage = 1;
	}
}

void Level::setPadsUp()
{

}

void Level::handleAnim(sf::Time t_deltaTime)
{

	if (m_frameHandler.current >= m_frameHandler.duration)
	{
		m_frameHandler.current = sf::Time::Zero;
		if (m_rockExplosion)
		{
			m_frameID++;
			if (m_frameID > 7)
			{
				//m_rockExplosion = false;
			}
		}
	}
	else if (!m_rockExplosion)
	{
		m_frameID = 0;
	}
	else
	{
		m_frameHandler.current += t_deltaTime;
		m_explosionSprite.setTextureRect(sf::IntRect(0 + 128 * m_frameID, 0, 128, 128));
	}
	
}