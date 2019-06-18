#pragma once

#include "app.h"

class pMain : public App
{
public:
	pMain();
	virtual ~pMain();
	virtual bool start();
	virtual void update(float deltaT);
	virtual void render();
	virtual void cleanup();
	static pMain &inst();

	sf::Sprite *m_backgroundSprite;

	//Mine
	sf::RenderWindow levelEditor_Window;
	virtual void RenderEditorWindow();
};
