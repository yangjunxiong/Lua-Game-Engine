#include "pch.h"
#include "ReactionBullet.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "CollisionManager.h"
#include "Entity.h"
#include "Player.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ReactionBullet);

ReactionBullet::ReactionBullet(RTTI::IdType type) :
	Reaction(type)
{
	Event<CollisionMessage>::Subscribe(*this);
}

ReactionBullet::ReactionBullet(const ReactionBullet& other) :
	Reaction(other)
{
	Event<CollisionMessage>::Subscribe(*this);
}

ReactionBullet::ReactionBullet(ReactionBullet&& other) :
	Reaction(move(other))
{
	Event<CollisionMessage>::Subscribe(*this);
}

ReactionBullet::~ReactionBullet()
{
	Event<CollisionMessage>::Unsubscribe(*this);
}

gsl::owner<Scope*> ReactionBullet::Clone() const
{
	return new ReactionBullet(*this);
}

void ReactionBullet::Notify(const BaseEvent& event)
{
	assert(event.Is(Event<CollisionMessage>::TypeIdClass()));
	auto& message = static_cast<const Event<CollisionMessage>&>(event).Message();
	auto entity1 = message.onHitEntity1;
	auto entity2 = message.onHitEntity2;

	if (entity1 == nullptr || entity2 == nullptr)
	{
		return;
	}

	assert(GetParent()->Is(Entity::TypeIdClass()));
	Entity& parent = static_cast<Entity&>(*GetParent());
	Entity* otherEntity = nullptr;
	if (entity1 == GetParent())
	{
		otherEntity = entity2;
	}
	else if (entity2 == GetParent())
	{
		otherEntity = entity1;
	}

	if (otherEntity != nullptr)
	{
		if (otherEntity->Name() == "Player2")
		{
			Player& player = static_cast<Player&>(*otherEntity);
			player.TakeDamage(1, *message.mWorldState);
			message.mWorldState->GetWorld()->Destroy(parent);
		}
		else if (otherEntity->Name() != "Player1")
		{
			message.mWorldState->GetWorld()->Destroy(parent);
		}
	}
}

const Vector<Attributed::Signature> ReactionBullet::Signatures()
{
	return Vector<Attributed::Signature>();
}