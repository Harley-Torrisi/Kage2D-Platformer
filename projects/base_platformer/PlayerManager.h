#pragma once
#include "example.h"
#include "GameObjectDefinition.h"
#include "objectNodeDefinition.h"
#include "PlayerObject.h"
#include "Level.h"

class PlayerManager
{
protected:
	sf::Clock m_clock;

public:
	PlayerManager(std::string path);
	~PlayerManager();

	virtual void start();
	virtual void update(float deltaT);
	virtual void render();

	bool p_GameReady = false;
	bool p_run = true;
	float posOffset;
	float scaleRacio;
	Level *p_level;
	PlayerObject *playerOne;
	PlayerObject *playerTwo;

	std::vector<GameObjectDefinition> worldObjects;
	std::vector<objectNodeDefinition> spawns;
};

