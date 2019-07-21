#include "rabbit.h"
#include "example.h"

Rabbit::Rabbit()
{
	m_sprite = kage::TextureManager::getSprite("data/zazaka.png");
	kage::centreOrigin(m_sprite);
	m_tags.add("rabbit");

	m_physicsStyle = GameObject::e_psBox2D;

	// Make a Box2D body
	m_body = kage::Physics::BodyBuilder()
				.pos(0, 0)
				.userData(this)	// This lets the body know which GameObject owns it
				.build();

	// Make a fixture (collision shape) for the body
	kage::Physics::CircleBuilder()
		.radius(0.4)
		.mass(1)
		.build(m_body); // We need to tell the builder which body to attach to
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
