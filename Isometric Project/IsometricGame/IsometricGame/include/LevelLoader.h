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
#include "SFML/Graphics.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "yaml-cpp/yaml.h"

// A struct to represent Terrain data in the level
struct TerrainData
{
	sf::Vector2f m_position;
};

// A struct to represent Obstacle data in the level
struct ObstacleData
{
	std::string m_type;
	sf::Vector2f m_position;
	double m_rotation;
	std::string m_alive;
};

// A struct to represent Player data in the level 
struct PlayerData
{
	sf::Vector2f m_position;
};

// A struct to represent Platform data in the level
struct PlatformData
{
	std::string m_type;
	sf::Vector2f m_position;
	double m_rotation;
};

// A struct to represent Missile data in the level
struct MissileData
{
	sf::Vector2f m_position;
};

 //A struct to represent Launcher data in the level
struct LauncherData
{
	sf::Vector2f m_position;
	double m_rotation;
};

// A struct to represent all the Level Data
struct LevelData
{
	std::vector<TerrainData> m_terrain;
	PlayerData m_player;
	std::vector<PlatformData> m_platform;
	std::vector<MissileData> m_missiles;
	LauncherData m_launcher;
	std::vector<ObstacleData> m_obstacles;
};

// A class to manage level loading using YAML
class LevelLoader
{
public:
	LevelLoader() = default;

	static bool load(int nr, LevelData& t_level);
};

