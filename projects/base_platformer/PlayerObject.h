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

	int speed = 5; //Get from game prefs later
	float decelSpeed = 5.0f;
	float jumpDampen = 0.05f;
	int jumpForce = 350;

	float bulletROF = 0.1f;
	float bulletTimer = 0;
};

