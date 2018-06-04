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

#include "LevelLoader.h"

/// <summary>
/// Extracts the player position values
/// </summary>
/// <param name="t_playerNode">A YAML node</param>
/// <param name="t_player">A simple struct to store the player data</param>
void operator >> (const YAML::Node& t_playerNode, PlayerData& t_player)
{
	t_player.m_position.x = t_playerNode["position"]["x"].as<float>();
	t_player.m_position.y = t_playerNode["position"]["y"].as<float>();
}

/// <summary>
/// Extracts the missile position values
/// </summary>
/// <param name="t_missileNode">A YAML node</param>
/// <param name="t_missile">A simple struct to store the missile data</param>
void operator >> (const YAML::Node& t_missileNode, MissileData& t_missile)
{
	t_missile.m_position.x = t_missileNode["position"]["x"].as<float>();
	t_missile.m_position.y = t_missileNode["position"]["y"].as<float>();
}

/// <summary>
/// Extracts the missile launcher position and rotation values
/// </summary>
/// <param name="t_launcherNode">A YAML node</param>
/// <param name="t_launcher">A simple struct to store the missle launcher data</param>
void operator >> (const YAML::Node& t_launcherNode, LauncherData& t_launcher)
{
	t_launcher.m_position.x = t_launcherNode["position"]["x"].as<float>();
	t_launcher.m_position.y = t_launcherNode["position"]["y"].as<float>();
	t_launcher.m_rotation = t_launcherNode["rotation"].as<double>();
}

/// <summary>
/// Extracts the platform type, position and rotation values
/// </summary>
/// <param name="t_platformNode">A YAML node</param>
/// <param name="t_platform">A simple struct to store the platform data</param>
void operator >> (const YAML::Node& t_platformNode, PlatformData& t_platform)
{
	t_platform.m_type = t_platformNode["type"].as<std::string>();
	t_platform.m_position.x = t_platformNode["position"]["x"].as<float>();
	t_platform.m_position.y = t_platformNode["position"]["y"].as<float>();
	t_platform.m_rotation = t_platformNode["rotation"].as<double>();
}

/// <summary>
/// Extracts the terrain position values
/// </summary>
/// <param name="t_terrainNode">A YAML node</param>
/// <param name="t_terrain">A simple struct to store the terrain data</param>
void operator >> (const YAML::Node& t_terrainNode, TerrainData& t_terrain)
{
	t_terrain.m_position.x = t_terrainNode["position"]["x"].as<float>();
	t_terrain.m_position.y = t_terrainNode["position"]["y"].as<float>();
}

/// <summary>
/// Extracts the Obstacle position values
/// </summary>
/// <param name="t_playerNode">A YAML node</param>
/// <param name="t_player">A simple struct to store the Obstacle data</param>
void operator >> (const YAML::Node& t_obstacleNode, ObstacleData& t_obstacle)
{
	t_obstacle.m_type = t_obstacleNode["type"].as<std::string>();
	t_obstacle.m_position.x = t_obstacleNode["position"]["x"].as<float>();
	t_obstacle.m_position.y = t_obstacleNode["position"]["y"].as<float>();
	t_obstacle.m_rotation = t_obstacleNode["rotation"].as<double>();
	t_obstacle.m_alive = t_obstacleNode["alive"].as<std::string>();
}

/// <summary>
/// Level function that extracts various game data from the YAML data structure
/// Invokes other functions to extract the player, missile, launcher, platforms and terrain data.
/// Because there are multiple platforms and terrain positions, the platform data and the terrain data
/// are stored in vectors.
/// </summary>
/// <param name="t_levelNode">A YAML node</param>
/// <param name="t_level">A simple struct to store data</param>
void operator >> (const YAML::Node& t_levelNode, LevelData& t_level)
{
	t_levelNode["player"] >> t_level.m_player;

	t_levelNode["launcher"] >> t_level.m_launcher;

	const YAML::Node& platformsNode = t_levelNode["platforms"].as<YAML::Node>();
	for (unsigned i = 0; i < platformsNode.size(); ++i)
	{
		PlatformData platform;
		platformsNode[i] >> platform;
		t_level.m_platform.push_back(platform);
	}

	const YAML::Node& terrainNode = t_levelNode["terrain"].as<YAML::Node>();
	for (unsigned i = 0; i < terrainNode.size(); ++i)
	{
		TerrainData terrain;
		terrainNode[i] >> terrain;
		t_level.m_terrain.push_back(terrain);
	}

	const YAML::Node& obstacleNode = t_levelNode["obstacles"].as<YAML::Node>();
	for (unsigned i = 0; i < obstacleNode.size(); ++i)
	{
		ObstacleData obstacle;
		obstacleNode[i] >> obstacle;
		t_level.m_obstacles.push_back(obstacle);
	}

	const YAML::Node& missileNode = t_levelNode["missile"].as<YAML::Node>();
	for (unsigned i = 0; i < missileNode.size(); ++i)
	{
		MissileData missile;
		missileNode[i] >> missile;
		t_level.m_missiles.push_back(missile);
	}
}

/// <summary>
/// Function to load the level from the YAML file
/// </summary>
/// <param name="t_level"></param>
/// <returns></returns>
bool LevelLoader::load(int nr, LevelData& t_level)
{
	std::stringstream ss;
	ss << "ASSETS/LEVELS/level";
	ss << nr;
	ss << ".yaml";

	t_level.m_terrain.clear();
	t_level.m_platform.clear();
	t_level.m_obstacles.clear();

	try
	{
		YAML::Node baseNode = YAML::LoadFile(ss.str());
		if (baseNode.IsNull())
		{
			std::string message("file: " + ss.str() + " not found");
			throw std::exception(message.c_str());
		}
		baseNode >> t_level;
	}
	catch (YAML::ParserException& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}

	return true;
}
