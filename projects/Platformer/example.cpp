#include "example.h"
#include "editorManager.h"
#include <thread>

Example::Example(): App()
{
}

Example::~Example()
{
}

Example &Example::inst()
{
	static Example s_instance;
	return s_instance;
}

bool Example::start()
{
	m_backgroundSprite = kage::TextureManager::getSprite("data/sky.jpg");
	sf::Vector2u resolution = m_backgroundSprite->getTexture()->getSize();
	m_backgroundSprite->setScale(float(m_window.getSize().x) / resolution.x, float(m_window.getSize().y) / resolution.y);

	Rabbit *rabbit = kage::World::build<Rabbit>();
	rabbit->position({ 960, 540 });
	return true;
}

void Example::update(float deltaT)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && m_window.hasFocus())
	{
		m_running = false;
	}
	ImGui::Begin("Kage2D");
	if (ImGui::Button("Exit"))
	{
		m_running = false;
	}

	if (ImGui::Button("Play"))
	{
	}

	if (ImGui::Button("Level Editor"))
	{
		appState = Edit;
		ImGui::End();
		ImGui::EndFrame();
		editorManager e_Managaer;
	}

	if (appState == None) {
		ImGui::End();
	}
	else
	{
		appState = None;
	}
}

void Example::render()
{
	m_window.draw(*m_backgroundSprite);
}

void Example::cleanup()
{
	delete m_backgroundSprite;
}

