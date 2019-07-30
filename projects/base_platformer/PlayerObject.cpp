#include "PlayerObject.h"
#include "example.h"
#include "windows.h"
#include "xinput.h"
#include "Bullet.h"


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
		//.type = b2_staticBody
		.userData(this)	// This lets the body know which GameObject owns it
		.fixedRotation(true)
		.build();
	
	// Make a fixture (collision shape) for the body
	kage::Physics::CircleBuilder()
		.radius(0.35f) // <- how would I pass a value from constructor using kaga::build<>()?
		.mass(1)
		.friction(0.0f)
		.build(m_body); // We need to tell the builder which body to attach to

	b2Fixture *sensor = kage::Physics::BoxBuilder()
		.size(kf::Vector2(0.6f, 0.35f))
		.pos(kf::Vector2(0.0f, 0.35f))
		.build(m_body);
	sensor->SetSensor(true);
	
}


PlayerObject::~PlayerObject()
{
}

void PlayerObject::update(float deltaT)
{
	timeMasterDude = deltaT;
	// Do logic here

	//if (animIndex > 3) // hard coded for now, now anim speed
	//	animIndex = 0;
	//kage::selectSpriteTile1D(m_sprite, animIndex, 48, 48);
	//animIndex++;

	
	MovePlayer();


	GameObject::update(deltaT);
}

void PlayerObject::onCollision(GameObject * obj)
{
	//GameObjectDef *def = (GameObjectDef)obj;
	if (obj->m_tags.has("death"))
	{
		m_dead = true;			// kills itself
		//obj->m_dead = true;	// kills the other object
	}
}


void PlayerObject::MovePlayer()
{
	bool canJump = (kage::Physics::getTouching(m_body, 0) != 0);

	//m_body->GetContactList()->contact->IsTouching()


	
	

	XINPUT_STATE controller;
	memset(&controller, 0, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState(playerIndex, &controller);

	//if (result != 0)
	//	return;

	b2Vec2 dir = b2Vec2(controller.Gamepad.sThumbRX / 32768.0f, controller.Gamepad.sThumbRY / -32768.0f);

	
	
	float angle = (dir.Length() > 0.2f) ? std::atan2(dir.y, dir.x) : m_sprite->getRotation()/57.29f; //

	

	//m_body->SetAngularVelocity(angle);
	//std::cout << angle << "\n";


	

	b2Vec2 myPos = m_body->GetPosition();
	
	

	b2Vec2 target = b2Vec2(0, 0);

	b2Vec2 vel = m_body->GetLinearVelocity();
	vel.x = 0;
	if (controller.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		
		vel.x = -speed;
	}
	else if (controller.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		vel.x = speed;
		
	}
	m_body->SetLinearVelocity(vel);

	//myPos.x = myPos.x + target.x;
	//myPos.y = 0;
	//m_body->SetTransform(myPos, angle);

	if (canJump && controller.Gamepad.bLeftTrigger != 0)
	{
		if (m_body->GetLinearVelocity().y < jumpDampen && m_body->GetLinearVelocity().y > -jumpDampen)
			m_body->ApplyForce(b2Vec2(0, -jumpForce), m_body->GetWorldCenter(), true);
	}

	m_rotation = angle;
	//m_body->SetTransform(m_body->GetPosition(), angle);

	bulletTimer += timeMasterDude;
	if (controller.Gamepad.bRightTrigger != 0 && bulletTimer > (timeMasterDude + bulletROF))
	{
		bulletTimer = timeMasterDude;
		Bullet *bullet = kage::World::build<Bullet>();
		bullet->parent = this;
		bullet->direction = b2Vec2(cos(angle), sin(angle));
		bullet->position(m_body->GetPosition());
	}
}