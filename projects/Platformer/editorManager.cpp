#include "editorManager.h"
#include "Level.h"
#include "tileDefinition.h"
#include "levelRenderTileDefinition.h"
#include <fstream>
#include <iostream>

bool run = true;
sf::RenderWindow window;
Level *level;


//https://opengameart.org/content/grid-background
sf::Sprite *backgroundChosen;
sf::Sprite *background01;
sf::Sprite *background01_Thumnail;
sf::Sprite *background02;
sf::Sprite *background02_Thumnail;

float thumSizeBackground = 100.0f;
float thumSizeTile = 50.0f;

int selectedTileIdex = 0;

enum EditorStates
{
	None,
	Paint,
	Move,
	Erase,
};
EditorStates editorState = None;

editorManager::editorManager()
{
	start();

	run = true;

	//ImGui::CreateContext();
	
	window.create(sf::VideoMode(1920, 1080), "Platform - Leve Editor");
	window.setFramerateLimit(60);
	ImGui::GetCurrentContext();
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
		window.resetGLStates();
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
	background02 = kage::TextureManager::getSprite("data/Platformer/background02.png");
	background02_Thumnail = kage::TextureManager::getSprite("data/Platformer/background02.png");
	sf::Vector2u resolution = background01->getTexture()->getSize();
	background01->setScale(float(window.getSize().x) / resolution.x, float(window.getSize().y) / resolution.y);
	background01_Thumnail->setScale(thumSizeBackground / resolution.x, thumSizeBackground / resolution.y);
	background02->setScale(float(window.getSize().x) / resolution.x, float(window.getSize().y) / resolution.y);
	background02_Thumnail->setScale(thumSizeBackground / resolution.x, thumSizeBackground / resolution.y);
	backgroundChosen = background01;

	level = new Level(*kage::TextureManager::getSprite("data/Platformer/spritesheet01.png"), 1920, 1080, *background01);
}

void editorManager::update(float deltaT)
{
	//Menu Bar
	ImGui::BeginMainMenuBar();

	if (ImGui::Button("Import"))
	{
	}
	if (ImGui::Button("Export"))
	{
		/*
		std::ofstream stream("hmantest.dat", std::ios::binary);
		stream.write((char *)&level, sizeof(level));
		std::cout << "Saved\n";

		Level *newl;
		std::ifstream ifs("hmantest.dat", std::ios::binary);

		ifs.read((char *)&newl, sizeof(newl));

		std::cout << "Loaded\n";

		std::cout << newl->atlasTileDefinitions.size();
		*/

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

	if (ImGui::ImageButton(*background01_Thumnail)) {
		backgroundChosen = background01;
		level->background = *background01;
	}
	ImGui::SameLine();
	if (ImGui::ImageButton(*background02_Thumnail)) {
		backgroundChosen = background02;
		level->background = *background02;
	}

	ImGui::Separator();
	

	ImGui::Text("Editor Options");
	ImGui::Columns(4);
	if (ImGui::Button("Nothing"))
	{
		editorState = None;
	}
	ImGui::NextColumn();
	if (ImGui::Button("Paint"))
	{
		editorState = Paint;
	}
	
	ImGui::NextColumn();
	if (ImGui::Button("Erase"))
	{
		editorState = Erase;
		selectedTileIdex = -1;;
	}
	ImGui::NextColumn();
	if (ImGui::Button("Move"))
	{
		editorState = Move;
	}
	ImGui::Columns(1);


	ImGui::Separator();

	ImGui::Text("Current Tile Index: " + selectedTileIdex);
	int col = ImGui::GetWindowSize().x / (level->tilePX + 10); //Additinal int is a column buffer to keep enough space to always have a square button
	if (col < 1)
		col = 1;
	ImGui::Columns(col);
	for(int i = 0; i < level->atlasTileDefinitions.size(); i++)
	{
		tileDefinition &tile = level->atlasTileDefinitions[i];
		
		ImGui::PushID(tile.atlasIndex);
		if (ImGui::ImageButton(tile.tileSprite)) {
			selectedTileIdex = tile.atlasIndex;
			std::cout << tile.atlasIndex;
		}
		ImGui::PopID();
		//ImGui::PushID(tile.atlasIndex);
		char buf[100];
		sprintf(buf, "Block##%d", i);
		ImGui::Checkbox(buf, &tile.blocking);
		//ImGui::PopID();
		sprintf(buf, "Kill##%d", i);
		//ImGui::PushID(tile.atlasIndex+1000);
		ImGui::Checkbox(buf, &tile.death);
		//ImGui::PopID();
		ImGui::NextColumn();
	}
	ImGui::Columns(1);
	ImGui::End();

	if (!ImGui::GetIO().WantCaptureMouse)
	{
		// The mouse isn’t over an ImGui window,
		// do SFML input.
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			int mouseX = sf::Mouse::getPosition().x - window.getPosition().x;
			int mouseY = sf::Mouse::getPosition().y - window.getPosition().y;

			int x = mouseX / level->tilePX;
			int y = mouseY / level->tilePX;

			int pointX = x * level->tilePX;
			int pointy = x * level->tilePX;


			//Implement linq select statement later!! This is dirty haha
			for (int i = 0; i < level->levelRenderTileDefinitions.size(); i++)
			{
				if (level->levelRenderTileDefinitions[i].x == x && level->levelRenderTileDefinitions[i].y == y)
				{
					level->levelRenderTileDefinitions[i].index = selectedTileIdex;
				}
			}

		}
	}

	
}

void editorManager::render()
{
	window.draw(*backgroundChosen);

	for (int i = 0; i < level->levelRenderTileDefinitions.size(); i++)
	{
		if (level->levelRenderTileDefinitions[i].index >= 0)
		{
			sf::Sprite drawSprite = level->atlasTileDefinitions[level->levelRenderTileDefinitions[i].index].tileSprite;
			drawSprite.setPosition(level->levelRenderTileDefinitions[i].x * level->tilePX, level->levelRenderTileDefinitions[i].y * level->tilePX);
			window.draw(drawSprite);
		}
	}
}

