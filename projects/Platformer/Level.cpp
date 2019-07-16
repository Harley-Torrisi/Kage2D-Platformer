#include "Level.h"
#include <fstream>
#include <iostream>


Level::Level()
{
	mapWidth = 1920;
	mapHeight = 1080;
	textureAtlas = *kage::TextureManager::getSprite("data/Platformer/tilesheet.png");

	background = *kage::TextureManager::getSprite("data/Platformer/background01.png");
	sf::Vector2u resolution = background.getTexture()->getSize();
	background.setScale(1920 / resolution.x, 1080 / resolution.y);

	tilePX = textureAtlas.getTexture()->getSize().x;
	tileCount = textureAtlas.getTexture()->getSize().y / tilePX;
	atlasTileDefinitions.resize(tileCount);
	
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
		atlasTileDefinitions[i].breakable = false;
		atlasTileDefinitions[i].tileSprite.setTexture(*textureAtlas.getTexture());
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
	float width	= ((float)mapWidth / tilePX) + tilePX;
	float height = ((float)mapHeight / tilePX) + tilePX;
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

class LevelData{ 
public:

	class RenderDef{
	public:
		int x;
		int y;
		int index;
	};

	class NodeDef {
	public:
		int x;
		int y;
		bool active;
	};

	class AtlasDef {
	public:
		bool isBlocking;
		bool isDeath;
	};

	int playerStartHealth;
	int playerMaxHealth;
	int playerMoveSpeed;
	int playerJumpForce;
	int shootDamage;
	int jumpOnTopDamage;
	int healPickupAmount;
};

//	This takes the selected csv files then iterates through the rows and colums to populate the level class.
//	String converters are used for values that represent numbers and bools. Bools are represented by 0 and 1
void Level::load(std::string path)
{
	//Game Settings
	std::string prefsPath = "data/Platformer/pformLevel_";
	prefsPath = prefsPath + std::string(path);
	prefsPath = prefsPath + "_GamePrefs";
	prefsPath = prefsPath + ".csv";
	std::fstream prefsFile;
	prefsFile.open(prefsPath, std::ios::in);
	for (int i = 0; i < levelRenderTileDefinitions.size(); i++)
	{
		std::string s_playerStartHealth;
		std::string s_playerMaxHealth;
		std::string s_playerMoveSpeed;
		std::string s_playerJumpForce;
		std::string s_shootDamage;
		std::string s_jumpOnTopDamage;
		std::string s_healPickupAmount;

		//	getLine will use the io file and read from its current point, storing whats read until it hits the assigned break point.
		std::getline(prefsFile, s_playerStartHealth, ',');
		std::getline(prefsFile, s_playerMaxHealth, ',');
		std::getline(prefsFile, s_playerMoveSpeed, ',');
		std::getline(prefsFile, s_playerJumpForce, ',');
		std::getline(prefsFile, s_shootDamage, ',');
		std::getline(prefsFile, s_jumpOnTopDamage, ',');
		std::getline(prefsFile, s_healPickupAmount, '\n');

		playerStartHealth = std::atoi(s_playerStartHealth.c_str());
		playerMaxHealth = std::atoi(s_playerMaxHealth.c_str());
		playerMoveSpeed = std::atoi(s_playerMoveSpeed.c_str());
		playerJumpForce = std::atoi(s_playerJumpForce.c_str());
		shootDamage = std::atoi(s_shootDamage.c_str());
		jumpOnTopDamage = std::atoi(s_jumpOnTopDamage.c_str());
		healPickupAmount = std::atoi(s_healPickupAmount.c_str());
	}

	
	//Render Definitions
	std::string renderPath = "data/Platformer/pformLevel_";
	renderPath = renderPath + std::string(path);
	renderPath = renderPath + "_RenderDef";
	renderPath = renderPath + ".csv";
	std::fstream renderFile;
	renderFile.open(renderPath, std::ios::in);
	for (int i = 0; i < levelRenderTileDefinitions.size(); i++)
	{
		std::string index;
		std::string x;
		std::string y;

		std::getline(renderFile, x, ',');
		std::getline(renderFile, y, ',');
		std::getline(renderFile, index, '\n');
		
		levelRenderTileDefinitions[i].x = std::atoi(x.c_str());
		levelRenderTileDefinitions[i].y = std::atoi(y.c_str());
		levelRenderTileDefinitions[i].index = std::atoi(index.c_str());

		//std::cout << index << "," << x << "," << y << "\n";
	}
	
	//Spawn Definitions
	std::string spawnPath = "data/Platformer/pformLevel_";
	spawnPath = spawnPath + std::string(path);
	spawnPath = spawnPath + "_SpawnDef";
	spawnPath = spawnPath + ".csv";
	std::fstream spawnFile;
	spawnFile.open(spawnPath, std::ios::in);
	for (int i = 0; i < spawnPointDefinitions.size(); i++)
	{
		std::string active;
		std::string x;
		std::string y;

		std::getline(spawnFile, x, ',');
		std::getline(spawnFile, y, ',');
		std::getline(spawnFile, active, '\n');

		spawnPointDefinitions[i].x = std::atoi(x.c_str());
		spawnPointDefinitions[i].y = std::atoi(y.c_str());
		std::istringstream(active) >> spawnPointDefinitions[i].active;
	}
	
	//Atlas Definitions
	std::string atlasPath = "data/Platformer/pformLevel_";
	atlasPath = atlasPath + std::string(path);
	atlasPath = atlasPath + "_AtlasDef";
	atlasPath = atlasPath + ".csv";
	std::fstream atlasFile;
	atlasFile.open(atlasPath, std::ios::in);
	for (int i = 0; i < atlasTileDefinitions.size(); i++)
	{
		std::string blocking;
		std::string death;
		std::string index;
		std::string breakable;

		std::getline(atlasFile, blocking, ',');
		std::getline(atlasFile, death, ',');
		std::getline(atlasFile, index, ',');
		std::getline(atlasFile, breakable, '\n');

		std::istringstream(blocking) >> atlasTileDefinitions[i].blocking;
		std::istringstream(death) >> atlasTileDefinitions[i].death;
		atlasTileDefinitions[i].atlasIndex = std::atoi(index.c_str());
		std::istringstream(breakable) >> atlasTileDefinitions[i].breakable;
	}
}

//	This takes a path and generates four csv files that make up the level.
//	Each CSV stores a different part; settings, atlasInfo, tileInfo, and spawnPosinfo.
//	It iterates through each value and writes to a file, in the case of tile bases settings;
//	This is done by saving to a colume, then moving onto the next column by using a coma and lew line to start a new row.
//  This is using the coma delimated file methode, meaning programs can read the file, saying when a comma is hit, stop and do something else like store then read the next value.
void Level::save(std::string path)
{
	//Game Settings
	std::string prefsPath = "data/Platformer/pformLevel_";
	prefsPath = prefsPath + std::string(path);
	prefsPath = prefsPath + "_GamePrefs";
	prefsPath = prefsPath + ".csv";
	std::fstream prefsFile;
	prefsFile.open(prefsPath, std::ios::out);
	for (int i = 0; i < levelRenderTileDefinitions.size(); i++)
	{
		prefsFile
			<< playerStartHealth << ","
			<< playerMaxHealth << ","
			<< playerMoveSpeed << ","
			<< playerJumpForce << ","
			<< shootDamage << ","
			<< jumpOnTopDamage << ","
			<< healPickupAmount << std::endl;
	}



	//Render Definitions
	std::string renderPath = "data/Platformer/pformLevel_";
	renderPath = renderPath + std::string(path);
	renderPath = renderPath + "_RenderDef";
	renderPath = renderPath + ".csv"; 
	std::fstream renderFile;
	renderFile.open(renderPath, std::ios::out);
	for (int i = 0; i < levelRenderTileDefinitions.size(); i++)
	{
		renderFile
			<< levelRenderTileDefinitions[i].x << ","
			<< levelRenderTileDefinitions[i].y << ","
			<< levelRenderTileDefinitions[i].index << std::endl;
	}
	
	//Spawn Definitions
	std::string spawnPath = "data/Platformer/pformLevel_";
	spawnPath = spawnPath + std::string(path);
	spawnPath = spawnPath + "_SpawnDef";
	spawnPath = spawnPath + ".csv";
	std::fstream spawnFile;
	spawnFile.open(spawnPath, std::ios::out);
	for (int i = 0; i < spawnPointDefinitions.size(); i++)
	{
		spawnFile
			<< spawnPointDefinitions[i].x << ","
			<< spawnPointDefinitions[i].y << ","
			<< spawnPointDefinitions[i].active << std::endl;
	}

	//Atlas Definitions
	std::string atlasPath = "data/Platformer/pformLevel_";
	atlasPath = atlasPath + std::string(path);
	atlasPath = atlasPath + "_AtlasDef";
	atlasPath = atlasPath + ".csv";
	std::fstream atlasFile;
	atlasFile.open(atlasPath, std::ios::out);
	for (int i = 0; i < atlasTileDefinitions.size(); i++)
	{
		atlasFile
			<< atlasTileDefinitions[i].blocking << ","
			<< atlasTileDefinitions[i].death << ","
			<< atlasTileDefinitions[i].atlasIndex << ","
			<< atlasTileDefinitions[i].breakable << std::endl;
	}
}

void Level::render(sf::RenderWindow &window)
{
	window.draw(background);

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
