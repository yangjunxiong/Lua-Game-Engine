#include "pch.h"
#include "CollisionManager.h"
#include "Event.h"
#include "EventMessageAttributed.h"

using namespace GameEngine;
using namespace std;

void GameEngine::CollisionManager::CreateInstance()
{
	if (sInstance == nullptr)
	{
		new CollisionManager();
	}
}

void GameEngine::CollisionManager::DestroyInstance()
{
	if (sInstance != nullptr)
	{
		delete sInstance;
	}
}

CollisionManager & GameEngine::CollisionManager::GetInstance()
{
	assert(sInstance != nullptr);
	return *sInstance;
}

CollisionManager::CollisionManager()
{
	sInstance = this;
	InitB2World();
}

CollisionManager::~CollisionManager()
{
	sInstance = nullptr;
	delete b2world;
}

b2Body * GameEngine::CollisionManager::CreateBody(const b2BodyDef & bodyDef)
{
	return b2world->CreateBody(&bodyDef);
}

void GameEngine::CollisionManager::RemoveBody(b2Body & body)
{
	b2world->DestroyBody(&body);
}

void GameEngine::CollisionManager::UpdateB2World(const float32& timeStep)
{
	b2world->Step(timeStep, 8, 3);
}

b2World * GameEngine::CollisionManager::GetB2World()
{
	return b2world;
}

void GameEngine::CollisionManager::InitB2World()
{
	b2Vec2 gravity(0, 0);
	b2world = new b2World(gravity);
}