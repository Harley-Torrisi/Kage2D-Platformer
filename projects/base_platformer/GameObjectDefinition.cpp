#include "GameObjectDefinition.h"
#include "example.h"


GameObjectDefinition::GameObjectDefinition(int x, int y, float scaleRatio, float offsetValue, bool collideable)
{
	//m_sprite = &sprite;
	m_physicsStyle = GameObject::e_psBox2D;

	collider2D = kage::Physics::BoxBuilder().pos(kf::Vector2(x / scaleRatio + offsetValue, y / scaleRatio + offsetValue)).size(1 / scaleRatio, 1 / scaleRatio).build(kage::Physics::getDefaultStatic());
	collider2D->SetSensor(!collideable);
	
}

GameObjectDefinition::~GameObjectDefinition()
{
}

void GameObjectDefinition::update(float deltaT)
{
	// Do logic here

	GameObject::update(deltaT);
}

void GameObjectDefinition::onCollision(GameObject * obj)
{
	//if (obj->m_tags.has("enemy"))
	//{
	//	m_dead = true;			// kills itself
	//	//obj->m_dead = true;	// kills the other object
	//}
}

void GameObjectDefinition::toggleCollision(bool collissionOn)
{
	//b2Fixture collider2D();
}
