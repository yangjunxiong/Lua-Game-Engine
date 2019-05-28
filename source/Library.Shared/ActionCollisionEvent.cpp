#include "pch.h"
#include "ActionCollisionEvent.h"
#include "CollisionManager.h"
#include "Box2D/Box2D.h"
#include "EventMessageAttributed.h"
#include "ActionCollision.h"
#include "Event.h"
#include "Entity.h"
#include "GameTime.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionCollisionEvent);

ActionCollisionEvent::ActionCollisionEvent(RTTI::IdType type) :
	ActionEvent(type)
{
}

void GameEngine::ActionCollisionEvent::Update(WorldState & state)
{
	CollisionManager::GetInstance().UpdateB2World(state.GetGameTime().ElapsedGameTime().count() / 10000000000.0f);

	auto b2world = CollisionManager::GetInstance().GetB2World();
	assert(b2world != nullptr);

	bool isColliding = false;
	for (auto contact = b2world->GetContactList(); contact; contact = contact->GetNext())
	{
		if (contact->IsTouching())
		{
			auto entity1 = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
			auto entity2 = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());
			CollisionMessage payload(*entity1, *entity2, state);
			shared_ptr<Event<CollisionMessage>> event = make_shared<Event<CollisionMessage>>(payload);
			state.GetWorld()->EnqueueEvent(event, chrono::milliseconds(mDelay));
			isColliding = true;
		}
	}

	if (isColliding)
	{
		UpdateTransform(state.GetGameTime().ElapsedGameTime());
	}
}

void GameEngine::ActionCollisionEvent::UpdateTransform(const std::chrono::milliseconds& timeStep)
{

	auto b2world = CollisionManager::GetInstance().GetB2World();
	assert(b2world != nullptr);

	for (auto body = b2world->GetBodyList(); body; body = body->GetNext())
	{
		auto entity = static_cast<Entity*>(body->GetUserData());
		if (mPositionMap.ContainsKey(entity))
		{
			body->SetLinearVelocity(b2Vec2(entity->GetPosition().x - mPositionMap[entity].x, entity->GetPosition().y - mPositionMap[entity].y));
		}

		mPositionMap[entity] = entity->GetPosition();
	}

	CollisionManager::GetInstance().UpdateB2World(timeStep.count() / 1000.0f);

	for (auto body = b2world->GetBodyList(); body; body = body->GetNext())
	{
		auto s = static_cast<Entity*>(body->GetUserData());
		assert(s->Is(Entity::TypeIdClass()));
		auto entity = static_cast<Entity*>(s);
		entity->SetPosition(glm::vec4(body->GetPosition().x, body->GetPosition().y, 0, 1));
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}
}

GameEngine::ActionCollisionEvent::~ActionCollisionEvent()
{
}

gsl::owner<Scope*> ActionCollisionEvent::Clone() const
{
	return new ActionCollisionEvent(*this);
}

const Vector<Attributed::Signature> ActionCollisionEvent::Signatures()
{
	return Vector<Attributed::Signature>();
}