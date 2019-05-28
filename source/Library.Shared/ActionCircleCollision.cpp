#include "pch.h"
#include "ActionCircleCollision.h"
#include "World.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "CollisionManager.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionCircleCollision);

ActionCircleCollision::ActionCircleCollision(RTTI::IdType type) :
	ActionCollision(type)
{
	InitBody();
}

void GameEngine::ActionCircleCollision::Update(WorldState & state)
{
	if (!mIsInitialized)
	{
		mBody->DestroyFixture(mBody->GetFixtureList());
		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = mIsSensor;
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0);
		circleShape.m_radius = mRadius;
		fixtureDef.shape = &circleShape;
		mBody->CreateFixture(&fixtureDef);

		if (mIsStatic)
		{
			mBody->SetType(b2BodyType::b2_staticBody);
		}
		else
		{
			mBody->SetType(b2BodyType::b2_dynamicBody);
		}

		mBody->GetFixtureList()->SetFriction(0.0f);
		mBody->SetUserData(GetParent());
		mIsInitialized = true;
	}

	ActionCollision::Update(state);
}

void GameEngine::ActionCircleCollision::InitBody()
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(0, 0);
	mBody = CollisionManager::GetInstance().CreateBody(bodyDef);
	b2FixtureDef fixtureDef;
	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = mRadius;
	fixtureDef.shape = &circleShape;
	mBody->CreateFixture(&fixtureDef);
	mBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	mBody->SetFixedRotation(true);
}

float GameEngine::ActionCircleCollision::GetRadius() const
{
	return mRadius;
}

gsl::owner<Scope*> ActionCircleCollision::Clone() const
{
	return new ActionCircleCollision(*this);
}

const Vector<Attributed::Signature> ActionCircleCollision::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Radius", Datum::DatumType::Float, 1, offsetof(ActionCircleCollision, mRadius))
		});
}
