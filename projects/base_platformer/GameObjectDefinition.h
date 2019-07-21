#pragma once
#include "kage2dutil/gameobject.h"

class GameObjectDefinition : public kage::GameObject
{
public:
	GameObjectDefinition(int x, int y, float scaleRatio, float offsetValue, bool collideable);
	~GameObjectDefinition();

	void update(float deltaT);
	void onCollision(GameObject *obj);

	int tileRef;

	b2Fixture *collider2D;

	void toggleCollision(bool collissionOn);
};

