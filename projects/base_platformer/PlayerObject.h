#pragma once
#include "kage2dutil/gameobject.h"

class PlayerObject : public kage::GameObject
{
public:
	PlayerObject();
	~PlayerObject();

	//void render();
	void update(float deltaT);
	void onCollision(GameObject *obj);

	void MovePlayer();

	sf::Sprite *charAtlas;

	int animIndex = 0;
	int playerIndex;

	float timeMasterDude;
};

