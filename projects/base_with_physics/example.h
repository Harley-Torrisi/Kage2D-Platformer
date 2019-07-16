#pragma once

#include "app.h"
#include "kage2dutil/physics.h"
#include "rabbit.h"

class Example : public App
{
public:
	Example();
	virtual ~Example();
	virtual bool start();
	virtual void update(float deltaT);
	virtual void render();
	virtual void cleanup();
	static Example &inst();

	sf::Sprite *m_backgroundSprite;
};
