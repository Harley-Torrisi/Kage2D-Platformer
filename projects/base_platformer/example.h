#pragma once

#include "app.h"
#include "rabbit.h"
#include "kage2dutil/physics.h"

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

	enum AppStates
	{
		None,
		Play,
		Edit
	};
	AppStates appState = None;
};
