#include "pMain.h"

pMain::pMain(): App()
{
}

pMain::~pMain()
{
}

pMain &pMain::inst()
{
	static pMain s_instance;
	return s_instance;
}

bool pMain::start()
{
	m_backgroundSprite = kage::TextureManager::getSprite("data/sky.jpg");
	sf::Vector2u resolution = m_backgroundSprite->getTexture()->getSize();
	m_backgroundSprite->setScale(float(m_window.getSize().x) / resolution.x, float(m_window.getSize().y) / resolution.y);
	return true;
}

void pMain::update(float deltaT)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && m_window.hasFocus())
	{
		m_running = false;
	}

	ImGui::Begin("Platformer");

	if (ImGui::Button("Play Level"))
	{
		
	}

	if (ImGui::Button("Create Level"))
	{
		if (!levelEditor_Window.isOpen()) {
			levelEditor_Window.create(sf::VideoMode(1920, 1080, 32), "Platformer - Level Editor");
			levelEditor_Window.setFramerateLimit(60);
			ImGui::SFML::Init(levelEditor_Window);
		}
	}

	if (ImGui::Button("Load Level"))
	{

	}

	   	
	if(ImGui::Button("Exit"))
	{ 
		m_running = false;
	}
	
	ImGui::End();
}

void pMain::render()
{
	m_window.draw(*m_backgroundSprite);
	if (levelEditor_Window.isOpen())
	{
		levelEditor_Window.draw(*m_backgroundSprite);
	}
}

void pMain::cleanup()
{
	delete m_backgroundSprite;
}

void pMain::RenderEditorWindow()
{
	while (levelEditor_Window.isOpen())
	{
		// Events are things such as keys being pressed, the window closing, etc.
		// There could be several events waiting for us, so use a loop to process them all.
		sf::Event event;
		while (levelEditor_Window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
			{
				levelEditor_Window.close();
				break;
			}
		}
		sf::Time deltaT_sfml = m_clock.restart();
		float deltaT = deltaT_sfml.asSeconds();
		ImGui::SFML::Update(levelEditor_Window, deltaT_sfml);

		kage::World::update(deltaT);
		update(deltaT);

		// Clear the window.
		levelEditor_Window.clear();

		render();
		kage::World::render(levelEditor_Window);

		ImGui::SFML::Render(levelEditor_Window);

		// Calling display will make the contents of the window appear on screen (before this, it was kept hidden in the back buffer).
		levelEditor_Window.display();
	}
}

