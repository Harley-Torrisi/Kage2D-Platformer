#include "rabbit.h"
#include "example.h"

Rabbit::Rabbit()
{
	m_sprite = kage::TextureManager::getSprite("data/zazaka.png");
	kage::centreOrigin(m_sprite);
	m_physicsStyle = GameObject::e_psNewtonian;
	m_tags.add("rabbit");
}

Rabbit::~Rabbit()
{

}

void Rabbit::update(float deltaT)
{
	// Do logic here

	GameObject::update(deltaT);
}

void Rabbit::onCollision(GameObject *obj)
{
	if (obj->m_tags.has("enemy"))
	{
		m_dead = true;			// kills itself
		//obj->m_dead = true;	// kills the other object
	}
}
