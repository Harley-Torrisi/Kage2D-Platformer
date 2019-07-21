#include "PlayerManager.h"
#include "example.h"
#include "level.h"
#include "PlayerObject.h"


sf::RenderWindow p_window;
//Level *p_level;
sf::String p_levelName;


PlayerManager::PlayerManager(std::string path)
{
	p_levelName = path;
	start();
	p_run = true;

	p_window.create(sf::VideoMode(1920, 1080), "Platform - Level Player");
	p_window.setFramerateLimit(60);
	ImGui::GetCurrentContext();
	ImGui::SFML::Init(p_window);
	while (p_window.isOpen()) {
		sf::Event event;
		while (p_window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				if (p_GameReady)
				{
					playerOne->m_dead = true;
					playerTwo->m_dead = true;
				}				
				p_run = false;
				p_window.close();
			}
		}
		if (p_run)
		{
			sf::Time deltaT_sfml = m_clock.restart();
			float deltaT = deltaT_sfml.asSeconds();
			ImGui::SFML::Update(p_window, deltaT_sfml);

			kage::World::update(deltaT);
			update(deltaT);

			p_window.clear();

			render();
			kage::World::render(p_window);

			ImGui::SFML::Render(p_window);
			p_window.display();
		}
		p_window.resetGLStates();
	}
}


PlayerManager::~PlayerManager()
{
}

void PlayerManager::start()
{
	p_level = new Level();
	p_level->load(p_levelName);

	kage::Physics::init(b2Vec2(0, 0.8));
	scaleRacio = 64 / p_level->tilePX;
	posOffset = p_level->tilePX / 64 / 2;

	for (int i = 0; i < p_level->levelRenderTileDefinitions.size(); i++)
	{
		levelRenderTileDefinition &renderDef = p_level->levelRenderTileDefinitions[i];		
		if (renderDef.index >= 0)
		{		
			tileDefinition &atlasDef = p_level->atlasTileDefinitions[renderDef.index];
			GameObjectDefinition objectDef(renderDef.x, renderDef.y, scaleRacio, posOffset, (atlasDef.blocking || atlasDef.death));
			objectDef.tileRef = i;
			if (atlasDef.blocking)
				objectDef.m_tags.add("blocking");
			if (atlasDef.breakable)
				objectDef.m_tags.add("breakable"); // Note to self: editor tool should auto true blocking when turned on.
			if (atlasDef.death)
				objectDef.m_tags.add("death");
			worldObjects.push_back(objectDef);
		}
		
	}

	//Rabbit *rabbit = kage::World::build<Rabbit>();
	//rabbit->position(11 / scaler + offset, 11 / scaler + offset); // Note that this now uses metres instead of pixels.

	for each (objectNodeDefinition node in p_level->spawnPointDefinitions)
	{
		if (node.active)
			spawns.push_back(node);
	}
}

void PlayerManager::update(float deltaT)
{
	kage::Physics::update(deltaT);

	if (!p_GameReady)
	{
		ImGui::OpenPopup("Intro");
		if (ImGui::BeginPopupModal("Intro"))
		{
			ImGui::Text("Some Instructions.... blah blah hoody ha");
			ImGui::Text("\n");
			if (ImGui::Button("Ready", ImVec2(-1.0f, 0.0f))) {
				p_GameReady = true;
				
				int ran = rand() % spawns.size(); // Note: maybe set editor to reuire at least two spawns?

				playerOne = kage::World::build<PlayerObject>();
				playerOne->position(spawns[ran].x / scaleRacio + posOffset, spawns[ran].y / scaleRacio + posOffset);

				int p1Ran = ran;
				while (p1Ran == ran)
					 ran = rand() % spawns.size();

				playerTwo = kage::World::build<PlayerObject>();
				playerTwo->position(spawns[ran].x / scaleRacio + posOffset, spawns[ran].y / scaleRacio + posOffset);
				playerTwo->m_sprite = kage::TextureManager::getSprite("data/Platformer/playerBlueSheet.png");
				kage::selectSpriteTile1D(playerTwo->m_sprite, 0, 48, 48);
				kage::centreOrigin(playerTwo->m_sprite);

			}
			ImGui::EndPopup();
		}
	}
	else
	{
		ImGui::SetNextWindowPos(ImVec2(10, p_window.getSize().y - 60));
		ImGui::SetNextWindowSize(ImVec2(250, 50));
		ImGui::Begin("Red Player");
		ImGui::ProgressBar(0.5f, ImVec2(235, 15), "Red:Health");
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(p_window.getSize().x / 2 - 40, p_window.getSize().y - 60));
		ImGui::SetNextWindowSize(ImVec2(80, 50));
		ImGui::Begin("Score");
		ImGui::Text("0:0");
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(p_window.getSize().x - 260, p_window.getSize().y - 60));
		ImGui::SetNextWindowSize(ImVec2(250, 50));
		ImGui::Begin("Blue Player");
		ImGui::ProgressBar(0.5f, ImVec2(235, 15), "Blue:Health");
		ImGui::End();
	}

}

void PlayerManager::render()
{
	p_level->render(p_window);
	kage::Physics::debugDraw(&p_window, 64);
}