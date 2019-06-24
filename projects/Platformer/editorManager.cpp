#include "editorManager.h"

bool run = true;
sf::RenderWindow window;

//https://opengameart.org/content/grid-background
sf::Sprite *background01;
sf::Sprite *background01_Thumnail;

editorManager::editorManager()
{
	start();

	run = true;

	ImGui::CreateContext();
	window.create(sf::VideoMode(1920, 1080), "Platform - Leve Editor");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				run = false;
				//ImGui::End();
				//ImGui::EndFrame();
				window.close();
			}
		}
		if (run)
		{
			sf::Time deltaT_sfml = m_clock.restart();
			float deltaT = deltaT_sfml.asSeconds();
			ImGui::SFML::Update(window, deltaT_sfml);

			kage::World::update(deltaT);
			update(deltaT);

			window.clear();

			render();
			kage::World::render(window);

			ImGui::SFML::Render(window);
			window.display();
		}
	}	
	//ImGui::SFML::Shutdown();
}


editorManager::~editorManager()
{
}

void editorManager::start()
{
	background01 = kage::TextureManager::getSprite("data/Platformer/background01.png");
	background01_Thumnail = kage::TextureManager::getSprite("data/Platformer/background01.png");
	sf::Vector2u resolution = background01->getTexture()->getSize();
	background01->setScale(float(window.getSize().x) / resolution.x, float(window.getSize().y) / resolution.y);
	background01_Thumnail->setScale(150.0f / resolution.x, 150.0f / resolution.y);
}

void editorManager::update(float deltaT)
{
	//Menu Bar
	ImGui::BeginMainMenuBar();
	if (ImGui::Button("New"))
	{
	}
	if (ImGui::Button("Import"))
	{
	}
	if (ImGui::Button("Export"))
	{
	}
	if (ImGui::Button("Exit"))
	{
		run = false;
		window.close();
	}
	ImGui::EndMainMenuBar();

	
	//Toolbox
	ImGui::Begin("Level Editor Tools");
	ImGui::Text("Set Background");
	ImGui::ImageButton(*background01_Thumnail);
	
	if (ImGui::Button("Import"))
	{
	}


	ImGui::End();
}

void editorManager::render()
{
	window.draw(*background01);
}

