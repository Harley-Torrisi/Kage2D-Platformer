#include "Level.h"



Level::Level(sf::Sprite templateSprite, int mapSizeX, int mapSizeY)
{
	setMapSize(mapSizeX, mapSizeY);
	tilePX = templateSprite.getTexture()->getSize().x;
	tileCount = templateSprite.getTexture()->getSize().y / tilePX;
	textureAtlas = templateSprite;
	atlasTileDefinitions.resize(tileCount);
	
	for (int i = 0; i <= atlasTileDefinitions.max_size(); i++)
	{
		tileDefinition *tile = new tileDefinition();
		tile->atlasIndex = i;
		tile->blocking = true;
		tile->death = false;
		//tile->tileSprite = templateSprite.setPosition?? i * tilePX + tilePX
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
