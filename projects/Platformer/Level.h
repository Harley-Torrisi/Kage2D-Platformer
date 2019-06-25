#pragma once
#include "app.h"
#include "tileDefinition.h"
#include "levelRenderTileDefinition.h"

class Level
{
public:
	Level(sf::Sprite &templateSprite, int mapSizeX, int mapSizeY, sf::Sprite &background);
	~Level();
	void clear(int tile);
	void load(std::string path);
	void save(std::string path);
	void render(sf::RenderWindow window);
	int getMapWidth();
	int getMapHeight();
	int getTileWidth();
	int getTileHeight();
	void setMapSize(int x, int y);
	void setTileSize(int x, int y);
	int getTile(int x, int y);
	void setTile(int tile, int x, int y);


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
};

