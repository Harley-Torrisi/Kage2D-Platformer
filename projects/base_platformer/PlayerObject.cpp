#include "PlayerObject.h"
#include "example.h"


PlayerObject::PlayerObject()
{

	m_sprite = kage::TextureManager::getSprite("data/Platformer/playerRedSheet.png");
	kage::selectSpriteTile1D(m_sprite, 0, 48, 48);
	
	kage::centreOrigin(m_sprite);
	m_tags.add("Player");

	m_physicsStyle = GameObject::e_psBox2D;

	// Make a Box2D body
	m_body = kage::Physics::BodyBuilder()
		.pos(0, 0)
		.userData(this)	// This lets the body know which GameObject owns it
		.build();

	// Make a fixture (collision shape) for the body
	kage::Physics::CircleBuilder()
		.radius(0.35f) // <- how would I pass a value from constructor using kaga::build<>()?
		.mass(1)
		.build(m_body); // We need to tell the builder which body to attach to
}


PlayerObject::~PlayerObject()
{
}

void PlayerObject::update(float deltaT)
{
	// Do logic here

	if (animIndex > 3) // hard coded for now, now anim speed
		animIndex = 0;
	kage::selectSpriteTile1D(m_sprite, animIndex, 48, 48);
	animIndex++;

	GameObject::update(deltaT);
}

void PlayerObject::onCollision(GameObject * obj)
{
	std::cout << "Colission\n";
	if (obj->m_tags.has("death"))
	{
		m_dead = true;			// kills itself
		//obj->m_dead = true;	// kills the other object
	}
}
