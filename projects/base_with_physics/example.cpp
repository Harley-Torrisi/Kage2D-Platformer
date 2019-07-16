#include "example.h"

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
	// Initialise the physics system. Set the default gravity to 9.8m/s^2 down.
	kage::Physics::init(b2Vec2(0, 9.8));
	// Floor, left wall and right wall static colliders.
	kage::Physics::BoxBuilder().pos(kf::Vector2(15, 16)).size(30, 1).build(kage::Physics::getDefaultStatic());
	kage::Physics::BoxBuilder().pos(kf::Vector2(0, 8)).size(1, 16).build(kage::Physics::getDefaultStatic());
	kage::Physics::BoxBuilder().pos(kf::Vector2(30, 8)).size(1, 16).build(kage::Physics::getDefaultStatic());

	m_backgroundSprite = kage::TextureManager::getSprite("data/sky.jpg");
	sf::Vector2u resolution = m_backgroundSprite->getTexture()->getSize();
	m_backgroundSprite->setScale(float(m_window.getSize().x) / resolution.x, float(m_window.getSize().y) / resolution.y);

	Rabbit *rabbit = kage::World::build<Rabbit>();
	rabbit->position(2, 4); // Note that this now uses metres instead of pixels.

	return true;
}

void Example::update(float deltaT)
{
	// You need to update the physics system every game update

	kage::Physics::update(deltaT);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && m_window.hasFocus())
	{
		m_running = false;
	}

	ImGui::Begin("Kage2D");
	if(ImGui::Button("Exit"))
	{ 
		m_running = false;
	}
	ImGui::End();
}

void Example::render()
{
	m_window.draw(*m_backgroundSprite);
	// The next line draws the physics debug info. This should be removed in a final release.
	kage::Physics::debugDraw(&m_window, 64);
}

void Example::cleanup()
{
	delete m_backgroundSprite;
}

