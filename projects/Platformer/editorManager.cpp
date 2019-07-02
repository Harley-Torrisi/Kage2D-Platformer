#include "editorManager.h"
#include "Level.h"
#include "tileDefinition.h"
#include "levelRenderTileDefinition.h"
#include <string>

bool run = true;
sf::RenderWindow window;
Level *level;

//https://opengameart.org/content/star
sf::Sprite *spawnIcon;
sf::Sprite *helperIcon;

float thumSizeBackground = 100.0f;
float thumSizeTile = 50.0f;

int selectedTileIdex = -0;

enum EditorStates {
	Paint,
	Spawn,
};
EditorStates editorState = Paint;

editorManager::editorManager()
{
	//	Render new window:
	//	TBH, direct copy from SFML and KAGE, was hard to get working and cant explain fully apart from certail calls will conflict with the first window.
	//	The corect methode would be to run multiple threads but that opens another can of worms, and I dont know how it would go with the kage interfance.
	//	So I have bastardised it to basicaly jump into the main windows drawing ImGui cycle, then jump out when done and continue the drawing of the main window
	//	Note, certain calls have been commented out, these are the one that would be need if done the right way.
	//	This may be the cause of my bug, and new window has to be opend twice before certain window rendering will occure.
	//	I have added the run bool which will stop any further rendering when the window gets closed, which stops the errors when something like windowRefIndex our of range or somehting.

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
	level = new Level();

	//Call after Level ^, needs the tilePX to calculate acurate sprite size.
	//	Creates drpites for courser helper, and spawn points. Done here and not level as it wont be visible in play mode.
	spawnIcon = kage::TextureManager::getSprite("data/Platformer/spawn01.png");
	sf::Vector2u spawnTexRes = spawnIcon->getTexture()->getSize();
	spawnIcon->setScale(level->tilePX / spawnTexRes.x, level->tilePX / spawnTexRes.y);
	helperIcon = kage::TextureManager::getSprite("data/Platformer/helper.png");
}

bool showExportPopup = false;
bool showImportPopup = false;
char dataPathText;
void editorManager::update(float deltaT)
{
#pragma region ImGuiTooplStrip

	//	Top ImGUI bar for import/export/exit options.
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::Button("Import Level"))
		{
			showImportPopup = true;
			ImGui::SetNextWindowPos(ImVec2(window.getSize().x / 2, window.getSize().y / 2));
			ImGui::SetNextWindowSize(ImVec2(window.getSize().x / 2, window.getSize().y / 5));
		}
		if (ImGui::Button("Export Level"))
		{
			showExportPopup = true;
			ImGui::SetNextWindowPos(ImVec2(window.getSize().x / 2, window.getSize().y / 2));
			ImGui::SetNextWindowSize(ImVec2(window.getSize().x / 2, window.getSize().y / 5));
		}

		if (ImGui::Button("Exit Editor"))
		{
			run = false;
			window.close();
		}
		ImGui::EndMainMenuBar();
	}

	//	showExportPopup and showImportPopup are bools to trigger the popup window for saving and loading levels.
	//	Cals Save/Load on the level class and passes a storage location.
	if (showExportPopup)
	{
		ImGui::OpenPopup("ExportPopup");
		if (ImGui::BeginPopupModal("ExportPopup"))
		{
			ImGui::Text("Select File Name, Letters Only.");
			ImGui::InputText("Select File Name", &dataPathText, 15);
			ImGui::Text("\n");
			ImGui::Text("\n");
			if(ImGui::Button("Cancel")) {
				showExportPopup = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {
				level->save(std::string(&dataPathText));
				showExportPopup = false;
			}
			ImGui::EndPopup();
		}
	}	
	if (showImportPopup)
	{
		ImGui::OpenPopup("ImportPopup");
		if (ImGui::BeginPopupModal("ImportPopup"))
		{
			ImGui::Text("Select File Name, Letters Only.");
			ImGui::InputText("Select File Name", &dataPathText, 15);
			ImGui::Text("\n");
			ImGui::Text("\n");
			if (ImGui::Button("Cancel")) {
				showImportPopup = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Import")) {
				level->load(std::string(&dataPathText));
				showImportPopup = false;
			}
			ImGui::EndPopup();
		}
	}

#pragma endregion

#pragma region ImGuiToolBox
	ImGui::Begin("Level Editor Tools");

	//	Gameplay Options
	//	These are saved in the level, idea being you can model your level and limitations around player settings.
	ImGui::Separator();
	ImGui::Text("Gameplay Settings:");
	ImGui::Text("\n");
	ImGui::SliderInt("Player Move Speed", &level->playerMoveSpeed, 250, 1000);
	ImGui::SliderInt("Player Jump Force", &level->playerJumpForce, 50, 200);
	ImGui::Text("\n");
	ImGui::SliderInt("Player Shoot Damage", &level->shootDamage, 1, 1000);
	ImGui::SliderInt("Player Jump Attack Damage", &level->jumpOnTopDamage, 0, 1000);
	ImGui::Text("\n");
	ImGui::SliderInt("Player Start Health", &level->playerStartHealth, 1, 1000);
	ImGui::SliderInt("Player Max Health", &level->playerMaxHealth, 1, 1000);
	ImGui::SliderInt("Health Pickup Amount", &level->healPickupAmount, 0, 1000);
	ImGui::Text("\n");
	ImGui::Separator();
	
	//	Editor Instructions
	ImGui::Text("\n");
	ImGui::Text("Editor Intructions:");
	ImGui::Text("\n");
	ImGui::Text("Left Click - Add\nRight Click - Remove");
	ImGui::Text("\n");
	ImGui::Text("Paint - Paint tiles\nSpawn - Add Spawn Points for gameplay");
	ImGui::Text("\n");
	ImGui::Text("Character in middle of screen used as reference for actual characters.");
	ImGui::Text("\n");
	ImGui::Separator();
	ImGui::Text("\n");

	//	Editor Mode Selector
	//	Switches edit states so your are not painting and spawning at the same time.
	ImGui::Columns(2);
	if (ImGui::Button("Paint Mode"))
	{
		editorState = Paint;
	}
	ImGui::NextColumn();
	if (ImGui::Button("Spawn Mode"))
	{
		editorState = Spawn;
	}
	ImGui::Columns(1);
	ImGui::Text("\n");
	ImGui::Separator();

	//	Tile Paint Selector
	//	This caluclates how many tiles will fit in the columns width. 
	//	Additinal int value is a column buffer to keep enough space to always have a square button, caused from decimal values in the calculation.
	//	Easier than rounding up. 25 should cover tiles up to 70px, currently using 48px tiles.
	int tileCols = ImGui::GetWindowSize().x / (level->tilePX + 25);
	if (tileCols < 1)
		tileCols = 1;
	ImGui::Columns(tileCols);
	//	Foreach atlas tile, a group of button and checkboxes are added to;
	//	select painting tile, and mark the properties of that tile group.
	// Each checkbox is linked to the value of that tile, changing its propertie when clicked and carrying along when serialized.
	for (int i = 0; i < level->atlasTileDefinitions.size(); i++)
	{
		tileDefinition &tile = level->atlasTileDefinitions[i];

		ImGui::PushID(tile.atlasIndex);
		if (ImGui::ImageButton(tile.tileSprite))
		{
			selectedTileIdex = tile.atlasIndex;
		}
		ImGui::PopID();

		char buf[100];
		sprintf(buf, "Block##%d", i);
		ImGui::Checkbox(buf, &tile.blocking);

		sprintf(buf, "Kill##%d", i);
		ImGui::Checkbox(buf, &tile.death);

		sprintf(buf, "Break##%d", i);
		ImGui::Checkbox(buf, &tile.breakable);
		ImGui::NextColumn();
	}
	ImGui::Columns(1);
#pragma endregion

	
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		//	Gets the mouse position relative to the screen, then subtracts the window position relative to the screen.
		//	This give the mouse pos of the window. If there is something in SFML that does that, more time wasited HAHA -_-
		//	Takes the pos, then devides by tilePX, this wil give a new pos that can be used to refrence tileDefinitions for other funtion calls.
		int mouseX = sf::Mouse::getPosition().x - window.getPosition().x;
		int mouseY = sf::Mouse::getPosition().y - window.getPosition().y;

		int x = mouseX / level->tilePX;
		int y = mouseY / level->tilePX;

		switch (editorState)
		{
		case Paint:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				SetRenderTileDef(x, y, selectedTileIdex);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
			{
				SetRenderTileDef(x, y, -1);
			}
			break;

		case Spawn:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				SetSpawnTileDef(x, y, true);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
			{
				SetSpawnTileDef(x, y, false);
			}
			break;

		default:
			break;
		}

		helperIcon->setPosition(x * level->tilePX, y * level->tilePX);
	}

	else
	{
		helperIcon->setPosition(-100, -100);
	}

	ImGui::End();
}

void editorManager::render()
{
	level->render(window);

	//Render spawn pos, done from editor as its not rendered in play mode.
	//For explanation, look at level->render as its the same logic.
	for (int i = 0; i < level->spawnPointDefinitions.size(); i++)
	{
		if (level->spawnPointDefinitions[i].active)
		{
			sf::Sprite drawSprite = *spawnIcon;
			drawSprite.setPosition(level->spawnPointDefinitions[i].x * level->tilePX, level->spawnPointDefinitions[i].y * level->tilePX);
			window.draw(drawSprite);
		}
	}

	window.draw(*helperIcon);
}

//	If a tile that matches the x,y is found, set its index to whatever painting tile is selected. Or -1 if removing.
void editorManager::SetRenderTileDef(int x, int y, int value)
{
	//Implement linq select statement later!! This is dirty haha
	for (int i = 0; i < level->levelRenderTileDefinitions.size(); i++)
	{
		if (level->levelRenderTileDefinitions[i].x == x && level->levelRenderTileDefinitions[i].y == y)
		{
			level->levelRenderTileDefinitions[i].index = value;
		}
	}
}

//	If a tile that matches the x,y is found, set its active to true or false.
void editorManager::SetSpawnTileDef(int x, int y, bool setActive = true)
{
	for (int i = 0; i < level->spawnPointDefinitions.size(); i++)
	{
		if (level->spawnPointDefinitions[i].x == x && level->spawnPointDefinitions[i].y == y)
		{
			level->spawnPointDefinitions[i].active = setActive;
		}
	}
}