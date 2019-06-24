#pragma once
#include "app.h"

class tileDefinition
{
public:
	tileDefinition();
	~tileDefinition();

	bool blocking;
	bool death;
	int atlasIndex;

	sf::Sprite tileSprite;
};

