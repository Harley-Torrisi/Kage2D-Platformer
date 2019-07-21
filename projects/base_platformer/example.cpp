#include "example.h"
#include "editorManager.h"
#include "PlayerManager.h"
#include <thread>
#include <string>
#include <iostream>
#include <filesystem>

sf::Sprite *playButtonIcon;
sf::Sprite *editButtonIcon;
sf::Sprite *exitButtonIcon;

Example::Example() : App()
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

std::string levelPath;
std::vector<std::string> levels;
bool selectLevel = false;
bool playLevel = false;

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
		levels.clear();
		std::string dir = "data/Platformer";
		for (const auto & file : std::experimental::filesystem::directory_iterator(dir))
		{
			std::string filePath = file.path().string();
			if (filePath.find("pformLevel_") != std::string::npos && filePath.find("_GamePrefs") != std::string::npos)
			{
				unsigned first = filePath.find("pformLevel_");
				unsigned last = filePath.find("_GamePrefs");
				levels.push_back(filePath.substr(first + 11, last - first - 11));
			}
		}
		ImGui::SetNextWindowPos(ImVec2(m_window.getSize().x / 2, m_window.getSize().y / 2));
		ImGui::SetNextWindowSize(ImVec2(m_window.getSize().x / 2, m_window.getSize().y / 5));
		selectLevel = true;
	}

	if (selectLevel)
	{
		ImGui::OpenPopup("SelectLevel");
		if (ImGui::BeginPopupModal("SelectLevel"))
		{
			for (int i = 0; i < levels.size(); i++)
			{
				if (ImGui::Button(levels[i].c_str(), ImVec2(-1.0f, 0.0f)))
				{
					levelPath = levels[i].c_str();
					playLevel = true;
					selectLevel = false;
				}
			}

			ImGui::Text("\n");

			if (ImGui::Button("Back", ImVec2(-1.0f, 0.0f))) {
				selectLevel = false;
			}
			ImGui::EndPopup();
		}
	}

	ImGui::SameLine();

	if (ImGui::ImageButton(*editButtonIcon))
	{
		appState = Edit;
		ImGui::End();
		ImGui::EndFrame();
		editorManager e_Managaer;
	}

	if (playLevel)
	{
		appState = Play;
		ImGui::End();
		ImGui::EndFrame();
		PlayerManager p_manager(levelPath);
		playLevel = false;
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

