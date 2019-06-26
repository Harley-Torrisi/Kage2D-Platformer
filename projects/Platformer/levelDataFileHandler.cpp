#include "levelDataFileHandler.h"
#include "level.h"
#include "levelDataModel.h"




levelDataFileHandler::levelDataFileHandler()
{
}


levelDataFileHandler::~levelDataFileHandler()
{
}

void levelDataFileHandler::SaveLevel(Level level, std::string path, std::string atlas, std::string background)
{
	
	levelDataModel model;
	model.atlasTexPath = atlas;
	model.backgroundTexPath = background;
	/*model.mapSizeX = level.mapWidth;
	model.mapSizeY = level.mapHeight;*/
	model.mapSizeX = 1920;
	model.mapSizeY = 1080;
	model.playerStartHealth = level.playerStartHealth;
	model.playerMaxHealth = level.playerMaxHealth;
	model.playerMoveSpeed = level.playerMoveSpeed;
	model.playerJumpForce = level.playerJumpForce;
	model.shootDamage = level.shootDamage;
	model.jumpOnTopDamage = level.jumpOnTopDamage;
	model.healPickupAmount = level.healPickupAmount;

	/*

	model.levelRenderTileDefinitions = level.levelRenderTileDefinitions;
	model.spawnPointDefinitions = level.spawnPointDefinitions;

	model.atlasTileDefinitions_Blocking.resize(level.atlasTileDefinitions.size());
	model.atlasTileDefinitions_Death.resize(level.atlasTileDefinitions.size());
	for (int i = 0; i < level.atlasTileDefinitions.size(); i++)
	{
		model.atlasTileDefinitions_Blocking[i] = level.atlasTileDefinitions[i].blocking;
		model.atlasTileDefinitions_Death[i] = level.atlasTileDefinitions[i].death;
	}

	*/

	std::ofstream ofs(path, std::ios::binary);
	ofs.write((char *)&model, sizeof(model));

	/*std::fstream file;
	file.open(path, std::ios::out | std::ios::binary);
	file.write((const char *)&model, sizeof(model));*/

	//Does this need to be an enumarator? Does write act like an "await"?
	//file.close();
}


Level levelDataFileHandler::LoadLevel(std::string path)
{
	if (path == "")
	{ 
		std::cout << "Error with path\n";
	}

	levelDataModel model;

	std::fstream file(path, std::ios::binary);
	file.read((char *)&model, sizeof(model));

	sf::Sprite *atlas = kage::TextureManager::getSprite(model.atlasTexPath);
	sf::Sprite *background = kage::TextureManager::getSprite(model.backgroundTexPath);

	std::cout << "MapX: " << model.mapSizeX;

	Level *level = new Level(*atlas, model.mapSizeX, model.mapSizeY, *background);

	/*

	level.levelRenderTileDefinitions = model.levelRenderTileDefinitions;
	level.spawnPointDefinitions = model.spawnPointDefinitions;

	for (int i = 0; i < level.atlasTileDefinitions.size(); i++)
	{
		level.atlasTileDefinitions[i].blocking = model.atlasTileDefinitions_Blocking[i];
		level.atlasTileDefinitions[i].death = model.atlasTileDefinitions_Death[i];
	}

	*/

	return *level;
}