#pragma once
#include "app.h"

class editorManager
{
protected:
	
	sf::Clock m_clock;
	sf::Font m_font;
	bool m_running;

public:
	editorManager();
	~editorManager();
	virtual void start();
	virtual void update(float deltaT);
	virtual void render();
	void SetRenderTileDef(int x, int y, int value);
	void SetSpawnTileDef(int x, int y, bool adding);
};

