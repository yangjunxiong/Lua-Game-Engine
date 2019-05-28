#pragma once
#include "WorldState.h"
#include "World.h"
#include <Box2D/Box2D.h>

namespace GameEngine
{
	class Entity;

	struct CollisionMessage
	{
	public:
		Entity* onHitEntity1;
		Entity* onHitEntity2;
		WorldState* mWorldState = nullptr;

		CollisionMessage(Entity& entity1, Entity& entity2, WorldState& state)
			: onHitEntity1(&entity1), onHitEntity2(&entity2), mWorldState(&state)
		{}
	};

	class CollisionManager
	{
	public:
		static void CreateInstance();
		static void DestroyInstance();
		static CollisionManager& GetInstance();

		b2Body* CreateBody(const b2BodyDef& bodyDef);
		void RemoveBody(b2Body& body);
		void UpdateB2World(const float32& timeStep = 0.0f);
		b2World* GetB2World();

	private:
		CollisionManager();
		CollisionManager(const CollisionManager& other) = delete;
		CollisionManager(CollisionManager&& other) = delete;
		CollisionManager& operator=(const CollisionManager& other) = delete;
		CollisionManager& operator=(CollisionManager&& other) = delete;
		~CollisionManager();
		static inline CollisionManager* sInstance = nullptr;

		void InitB2World();
		b2World* b2world = nullptr;
	};
}