#include "Bullet.h"



Bullet::Bullet()
{
	m_sprite = kage::TextureManager::getSprite("data/Platformer/bullet.png");

	kage::centreOrigin(m_sprite);
	m_tags.add("Bullet");

	m_physicsStyle = GameObject::e_psBox2D;

	// Make a Box2D body
	m_body = kage::Physics::BodyBuilder()
		.pos(0, 0)
		//.type(b2_kinematicBody)
		.userData(this)	// This lets the body know which GameObject owns it
		.fixedRotation(true)
		
		.build();

	// Make a fixture (collision shape) for the body
	b2Fixture *sensor = kage::Physics::CircleBuilder()
		.radius(0.25f) // <- how would I pass a value from constructor using kaga::build<>()?
		.mass(1)
		
		.friction(0.0f)
		.build(m_body); // We need to tell the builder which body to attach to
	sensor->SetSensor(true);
		
}


Bullet::~Bullet()
{

}

void Bullet::update(float deltaT)
{
	//m_body->ApplyForce(direction, m_body->GetWorldCenter(), true);
	m_body->SetLinearVelocity(b2Vec2(direction.x * 25, direction.y * 25));
	m_rotation = std::atan2(direction.y, direction.x);
	GameObject::update(deltaT);
}

void Bullet::onCollision(GameObject * obj)
{
	if (obj == parent)
		return;

	//obj->m_tags.add("blocking");
	//m_dead = true;
	//GameObjectDef *def = (GameObjectDef)obj;
	if (obj->m_tags.has("blocking"))
	{
		m_dead = true;			// kills itself
		//obj->m_dead = true;	// kills the other object
	}
}
