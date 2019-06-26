#pragma once
#include "app.h"
#include "tileDefinition.h"
#include "levelRenderTileDefinition.h"
#include "objectNodeDefinition.h"

class Level
{
public:
	Level(sf::Sprite &templateSprite, int mapSizeX, int mapSizeY, sf::Sprite &background);
	~Level();
	void clear(int tile);
	void load(std::string path);
	void save(std::string path);
	void render(sf::RenderWindow &window);

	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
	std::vector<int> tiles;
	sf::Sprite textureAtlas;
	std::vector<tileDefinition> tileDefinitions;
	std::vector<levelRenderTileDefinition> levelRenderTileDefinitions;
	sf::Sprite background;
	std::vector<tileDefinition> atlasTileDefinitions;
	float tilePX;
	int tileCount;
	std::vector<objectNodeDefinition> spawnPointDefinitions;

	int playerStartHealth = 500;
	int playerMaxHealth = 1000;
	int playerMoveSpeed = 500;
	int playerJumpForce = 100;
	int shootDamage = 100;
	int jumpOnTopDamage = 0;
	int healPickupAmount = 200;
};

