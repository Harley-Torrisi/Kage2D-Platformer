#pragma once
#include <string>
#include "app.h"
#include "tileDefinition.h"
#include "levelRenderTileDefinition.h"
#include "objectNodeDefinition.h"

using namespace std;

class levelDataModel
{
public:
	string atlasTexPath;
	string backgroundTexPath;
	int mapSizeX;
	int mapSizeY;

	int playerStartHealth;
	int playerMaxHealth;
	int playerMoveSpeed;
	int playerJumpForce;
	int shootDamage;
	int jumpOnTopDamage;
	int healPickupAmount;

	//std::vector<levelRenderTileDefinition> levelRenderTileDefinitions;
	//std::vector<objectNodeDefinition> spawnPointDefinitions;
	//std::vector<bool> atlasTileDefinitions_Blocking;
	//std::vector<bool> atlasTileDefinitions_Death;
};

