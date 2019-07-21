#pragma once
#include "app.h"

class tileDefinition
{
public:
	tileDefinition();
	~tileDefinition();

	//Tags that will be associated to the game object
	bool blocking;
	bool death;
	bool breakable;
	
	int atlasIndex;

	sf::Sprite tileSprite;
};

