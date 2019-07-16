////////////////////////////////////////////////////////////
// KAGE2D
// Copyright (C) 2019 Kojack (rajetic@gmail.com)
//
// KAGE2D is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#ifndef KAGE_PHYSICS_HEADER
#define KAGE_PHYSICS_HEADER
#include "box2d/Box2D.h"
#include "kf/kf_vector2.h"

namespace sf
{
	class RenderWindow;
}

namespace kage
{
	class GameObject;

	namespace Physics
	{
		// Hit is used to hold raycast intersection results.
		struct Hit
		{
		public:
			kf::Vector2 pos;	// Position of intersection.
			float distance;		// Proportion of the ray at which the intersection occured.
			bool hit;			// Bool to say if an intersection happened.
			b2Fixture *fixture;		// The collider that was intersected.

			Hit() : pos(kf::Vector2(0, 0)), distance(0), hit(false), fixture(0) {}
		};


		void init(b2Vec2 gravity = b2Vec2(0,0));	// Initialise the physics system.
		void update(float dt);						// Step the physics system.
		void gravity(b2Vec2 gravity);
		b2Vec2 gravity();
		b2World *getWorld();						// Retrieve the Box2D world.
		void debugDraw(sf::RenderWindow *win, float scale);							// Draw debug information.
		b2Fixture *createCircle(b2Body *body, float radius, kf::Vector2f offset, float density, float damping, void *userData, float restitution, float friction);		// Create a circle collider.
		b2Fixture *createBox(b2Body *body, kf::Vector2f size, kf::Vector2f offset, float angle, float density, float damping, void *userData, float restitution, float friction);	// Create a box collider.
		b2Fixture *createEdge(b2Body *body, kf::Vector2f vertex1, kf::Vector2f vertex2, float density, float damping, void *userData, float restitution, float friction);	// Create an edge collider.
		b2Body *createBody(kf::Vector2f pos, float angle = 0, b2BodyType type = b2BodyType::b2_dynamicBody, bool fixedRotation = false, void *userData = 0);
		Hit rayCast(kf::Vector2 p1, kf::Vector2 p2);	// Perform a ray cast. Only the closest intersection is returned.
		//void setActive(b2Body *b, bool active);
		b2Body* getDefaultStatic();
		GameObject *getGameObject(b2Fixture *fixture);
		GameObject *getGameObject(b2Body *fixture);
		b2Fixture *getTouching(b2Body *body, int fixtureNum);

		// Fluent Circle collider builder helper
		class CircleBuilder
		{
		public:
			kf::Vector2 m_pos;
			float		m_mass;
			float		m_density;
			float		m_radius;
			float		m_damping;
			void	   *m_userData;
			b2BodyType	m_type;
			bool		m_calcDensity;
			float		m_restitution;
			float       m_friction;

		public:
			CircleBuilder() : m_pos(kf::Vector2(0, 0)),
				m_radius(1.0f),
				m_mass(0),
				m_density(1),
				m_damping(0),
				m_userData(0),
				m_calcDensity(false),
				m_restitution(0), 
				m_friction(0)
			{
			}

			CircleBuilder &pos(const kf::Vector2 &p);
			CircleBuilder &pos(float x, float y);
			CircleBuilder &radius(float r);
			CircleBuilder &mass(float m);
			CircleBuilder &density(float d);
			CircleBuilder &damping(float d);
			CircleBuilder &userData(void *ud);
			CircleBuilder &restitution(float r);
			CircleBuilder &friction(float f);
			b2Fixture *build(b2Body *body);
		};

		// Fluent Box collider builder helper
		class BoxBuilder
		{
		public:
			kf::Vector2 m_pos;
			float		m_mass;
			float		m_density;
			kf::Vector2 m_size;
			float		m_damping;
			void	   *m_userData;
			bool		m_calcDensity;
			float		m_restitution;
			float       m_angle;
			float       m_friction;

		public:
			BoxBuilder() : m_pos(kf::Vector2(0, 0)), m_size(kf::Vector2(1, 1)), m_angle(0), m_mass(0), m_density(1), m_damping(0), m_userData(0), m_calcDensity(false), m_restitution(0), m_friction(0)
			{
			}

			BoxBuilder &pos(const kf::Vector2 &p);
			BoxBuilder &pos(float x, float y);
			BoxBuilder &size(float x, float y);
			BoxBuilder &size(float s);
			BoxBuilder &size(const kf::Vector2 &s);
			BoxBuilder &angle(float a);
			BoxBuilder &mass(float m);
			BoxBuilder &density(float d);
			BoxBuilder &damping(float d);
			BoxBuilder &userData(void *ud);
			BoxBuilder &restitution(float r);
			BoxBuilder &friction(float f);
			b2Fixture *build(b2Body *body);
		};

		class BodyBuilder
		{
		public:
			b2BodyType m_type;
			kf::Vector2 m_pos;
			void	   *m_userData;
			float       m_angle;
			bool m_fixedRotation;

			BodyBuilder() : m_type(b2BodyType::b2_dynamicBody), m_userData(0), m_angle(0)
			{
			}

			BodyBuilder &type(b2BodyType type);
			BodyBuilder &pos(const kf::Vector2 &p);
			BodyBuilder &pos(float x, float y);
			BodyBuilder &angle(float a);
			BodyBuilder &userData(void *ud);
			BodyBuilder &fixedRotation(bool fr);
			b2Body *build();
		};
	}
}

#endif