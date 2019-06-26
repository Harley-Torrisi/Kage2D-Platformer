#include "Level.h"



Level::Level(sf::Sprite &templateSprite, int mapSizeX, int mapSizeY, sf::Sprite &backgroundSprite)
{
	mapWidth = mapSizeX;
	mapHeight = mapSizeY;
	tilePX = templateSprite.getTexture()->getSize().x;
	tileCount = templateSprite.getTexture()->getSize().y / tilePX;
	textureAtlas = templateSprite;
	atlasTileDefinitions.resize(tileCount);
	background = backgroundSprite;
	
	//	Atlas Tile definitions are the tiles stored to represent the painting tiles, not whats on the map.
	//	When a new level class is constructed it requires an atlas. So its ready of the bat.
	//	This here will itterate though how many tiles in the atlas there are, calculated above ^
	//	Then; set a matching index to be refrenced later, set default properties like blocking,
	//	and set the texure/position to allign with the tile in the atlas, using (kage::selectSpriteTile1D)
	for (int i = 0; i < atlasTileDefinitions.size(); i++)
	{
		atlasTileDefinitions[i].atlasIndex = i;
		atlasTileDefinitions[i].blocking = true;
		atlasTileDefinitions[i].death = false;
		atlasTileDefinitions[i].tileSprite.setTexture(*templateSprite.getTexture());
		kage::selectSpriteTile1D(&atlasTileDefinitions[i].tileSprite, i, tilePX, tilePX);
	}

	//	Here we take the windowWidth/windowHeight, divide it by the tile size to get a number of tiles required to fill the screen.
	//	The values that represent heigh/width have an additional value of tileSize added,
	//	this is to deal with the rounding as the current methode, does not account for floating points between 0.1 - 0.75.
	//	Or whatever the rounding down is :)
	//	An array length is then generated, and some wizardry is used to calculate the x,y cords for each tile in the array.
	//	index of -1 one is used to flag the tile as dont draw.
	//	Any methode trying to cordinate with this array will need to divide the mouse cord by tilePX, as to match up.
	//	This methode is better as it acts as if there is snapping, and limits the itterations through the array giving better performance.
	float width	= ((float)mapSizeX / tilePX) + tilePX;
	float height = ((float)mapSizeY / tilePX) + tilePX;
	levelRenderTileDefinitions.resize(width * height);
	spawnPointDefinitions.resize(width * height);
	for (int i = 0; i < levelRenderTileDefinitions.size(); i++)
	{
		levelRenderTileDefinitions[i].x = i % (int)width;
		levelRenderTileDefinitions[i].y = i / width;
		levelRenderTileDefinitions[i].index = -1;

		spawnPointDefinitions[i].x = i % (int)width;
		spawnPointDefinitions[i].y = i / width;
		spawnPointDefinitions[i].active = false;
	}
}


Level::~Level()
{
}

void Level::clear(int atlasTileIndex = -1)
{
	//This will by default set every tile ref to dont draw, or all one tile of assigned.
	for (int i = 0; i < levelRenderTileDefinitions.size(); i++)
	{
		levelRenderTileDefinitions[i].index = atlasTileIndex;
	}
}

void Level::load(std::string path)
{

}

void Level::save(std::string path)
{

}

void Level::render(sf::RenderWindow &window)
{
	//The renderinf of tiles is done by fidning tileDefs that have a tile index higher than >= 0
	//-1 being dont draw anything, 0 and up being a refrence to a tile in the atlas.
	//If one is found it uses the index to take a refrence to the sprite, sets the position, then calls the draw funtion to pait to screen.
	for (int i = 0; i < levelRenderTileDefinitions.size(); i++)
	{
		if (levelRenderTileDefinitions[i].index >= 0)
		{
			sf::Sprite drawSprite = atlasTileDefinitions[levelRenderTileDefinitions[i].index].tileSprite;
			drawSprite.setPosition(levelRenderTileDefinitions[i].x * tilePX, levelRenderTileDefinitions[i].y * tilePX);
			window.draw(drawSprite);
		}
	}
}
