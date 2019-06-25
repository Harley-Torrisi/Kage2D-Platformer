#include "Level.h"



Level::Level(sf::Sprite &templateSprite, int mapSizeX, int mapSizeY, sf::Sprite &backgroundSprite)
{
	
	setMapSize(mapSizeX, mapSizeY);
	tilePX = templateSprite.getTexture()->getSize().x;
	tileCount = templateSprite.getTexture()->getSize().y / tilePX;
	textureAtlas = templateSprite;
	atlasTileDefinitions.resize(tileCount);
	background = backgroundSprite;
	
	for (int i = 0; i < atlasTileDefinitions.size(); i++)
	{
		atlasTileDefinitions[i].atlasIndex = i;
		atlasTileDefinitions[i].blocking = true;
		atlasTileDefinitions[i].death = false;
		atlasTileDefinitions[i].tileSprite.setTexture(*templateSprite.getTexture());
		kage::selectSpriteTile1D(&atlasTileDefinitions[i].tileSprite, i, tilePX, tilePX);
	}
	
	int index = 0;
	int colX = mapSizeX / tilePX;
	int colY = mapSizeY / tilePX;
	levelRenderTileDefinitions.resize(colX * colY);

	for (float y = 0; y < colY; y ++)
	{
		for (float x = 0; x < colX; x ++)
		{
			levelRenderTileDefinitions[index].x = x;
			levelRenderTileDefinitions[index].y = y;
			levelRenderTileDefinitions[index].index = -1; // -1 means dont draw
			index++;
		}
	}
}


Level::~Level()
{
}

void Level::clear(int tile = -1)
{
	for (int y = 0; y < mapHeight -1; y++)
	{
		for (int x = 0; x < mapWidth -1; x++)
		{
			setTile(tile, x, y);
		}
	}
}

void Level::load(std::string path)
{

}

void Level::save(std::string path)
{

}

void Level::render(sf::RenderWindow window)
{
	for (int y = 0; y < mapHeight - 1; y++)
	{
		for (int x = 0; x < mapWidth - 1; x++)
		{
			/*
			int tile = getTile(x, y); Read tile int from map at x,y
			Look up definition using the tile int.
			Set texture rect with selectSpriteTile2D
			Set sprite position to x*tileWidth, y*tileheight
			window.draw(*spriet); Give the sprite to the window’s draw method
			*/
		}
	}
}

int Level::getMapWidth()
{
	return mapWidth;
}
int Level::getMapHeight()
{
	return mapHeight;
}

int Level::getTileWidth()
{
	return tileWidth;
}
int Level::getTileHeight()
{
	return tileHeight;
}

void Level::setMapSize(int x, int y)
{
	mapWidth = x;
	mapHeight = y;
}

void Level::setTileSize(int x, int y)
{
	tileWidth = x;
	tileHeight = y;
}

int Level::getTile(int x, int y)
{
	return (x + y * mapWidth);
}

void Level::setTile(int tile , int x, int y)
{

}
