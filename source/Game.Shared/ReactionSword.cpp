#include "pch.h"
#include "ReactionSword.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "CollisionManager.h"
#include "Entity.h"
#include "Player.h"
#include "GameTime.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ReactionSword);

ReactionSword::ReactionSword(RTTI::IdType type) :
	Reaction(type)
{
	Event<CollisionMessage>::Subscribe(*this);
}

ReactionSword::ReactionSword(const ReactionSword& other) :
	Reaction(other)
{
	Event<CollisionMessage>::Subscribe(*this);
}

ReactionSword::ReactionSword(ReactionSword&& other) :
	Reaction(move(other))
{
	Event<CollisionMessage>::Subscribe(*this);
}

ReactionSword::~ReactionSword()
{
	Event<CollisionMessage>::Unsubscribe(*this);
}

gsl::owner<Scope*> ReactionSword::Clone() const
{
	return new ReactionSword(*this);
}

void ReactionSword::Notify(const BaseEvent& event)
{
	Player& parent = static_cast<Player&>(*GetParent());
	if (parent.GetHealth() <= 0)
	{
		return;
	}

	assert(event.Is(Event<CollisionMessage>::TypeIdClass()));
	auto& message = static_cast<const Event<CollisionMessage>&>(event).Message();
	auto entity1 = message.onHitEntity1;
	auto entity2 = message.onHitEntity2;

	if (entity1 == nullptr || entity2 == nullptr)
	{
		return;
	}

	Entity* otherEntity = nullptr;
	if (entity1 == &parent)
	{
		otherEntity = entity2;
	}
	else if (entity2 == &parent)
	{
		otherEntity = entity1;
	}

	if (otherEntity != nullptr && otherEntity->Name() == "Player1")
	{
		Player& player = static_cast<Player&>(*otherEntity);
		auto now = message.mWorldState->GetGameTime().CurrentTime();
		auto duration = (now - mLastTouchTime).count() / 1000000;
		if (duration > 1000)
		{
			mLastTouchTime = now;
			player.TakeDamage(1, *message.mWorldState);
		}
	}
}

const Vector<Attributed::Signature> ReactionSword::Signatures()
{
	return Vector<Attributed::Signature>();
}