#pragma once
#include "kage2dutil/gameobject.h"
#include "example.h"

class Bullet : public kage::GameObject
{
public:
	Bullet();
	~Bullet();

	//void render();
	void update(float deltaT);
	void onCollision(GameObject *obj);

	b2Vec2 direction;
	GameObject *parent;
};

