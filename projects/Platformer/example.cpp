#include "example.h"
#include "editorManager.h"
#include <thread>

sf::Sprite *playButtonIcon;
sf::Sprite *editButtonIcon;
sf::Sprite *exitButtonIcon;

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
	m_backgroundSprite = kage::TextureManager::getSprite("data/Platformer/background02.png");
	sf::Vector2u resolution = m_backgroundSprite->getTexture()->getSize();
	m_backgroundSprite->setScale(float(m_window.getSize().x) / resolution.x, float(m_window.getSize().y) / resolution.y);

	playButtonIcon = kage::TextureManager::getSprite("data/Platformer/play.png");
	editButtonIcon = kage::TextureManager::getSprite("data/Platformer/edit.png");
	exitButtonIcon = kage::TextureManager::getSprite("data/Platformer/exit.png");

	return true;
}

void Example::update(float deltaT)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && m_window.hasFocus())
	{
		m_running = false;
	}

	ImGui::SetNextWindowPos(ImVec2(550, 550));
	ImGui::SetNextWindowSize(ImVec2(825, 300));

	
	ImGui::Begin("Platformer - by Harley Torrisi (2019)");
	//Weird, but will break if I put this button last.
	if (ImGui::ImageButton(*exitButtonIcon))
	{
		m_running = false;
	}

	ImGui::SameLine();

	if (ImGui::ImageButton(*playButtonIcon))
	{
	}

	ImGui::SameLine();

	if (ImGui::ImageButton(*editButtonIcon))
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

