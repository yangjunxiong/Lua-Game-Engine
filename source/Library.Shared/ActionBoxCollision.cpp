#include "pch.h"
#include "ActionBoxCollision.h"
#include "World.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "CollisionManager.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionBoxCollision);

ActionBoxCollision::ActionBoxCollision(RTTI::IdType type) :
	ActionCollision(type)
{
	InitBody();
}

void ActionBoxCollision::Update(class WorldState& state)
{
	if (!mIsInitialized)
	{
		mBody->DestroyFixture(mBody->GetFixtureList());
		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = mIsSensor;
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(mWidth, mLength);
		fixtureDef.shape = &polygonShape;
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

void GameEngine::ActionBoxCollision::InitBody()
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(0, 0);
	mBody = CollisionManager::GetInstance().CreateBody(bodyDef);
	b2FixtureDef fixtureDef;
	b2PolygonShape polygonShape;
	fixtureDef.shape = &polygonShape;
	mBody->CreateFixture(&fixtureDef);
	mBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	mBody->SetFixedRotation(true);
}

float GameEngine::ActionBoxCollision::GetWidth() const
{
	return mWidth;
}

float GameEngine::ActionBoxCollision::GetLength() const
{
	return mLength;
}

gsl::owner<Scope*> ActionBoxCollision::Clone() const
{
	return new ActionBoxCollision(*this);
}

const Vector<Attributed::Signature> ActionBoxCollision::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Width", Datum::DatumType::Float, 1, offsetof(ActionBoxCollision, mWidth)),
		Signature("Length", Datum::DatumType::Float, 1, offsetof(ActionBoxCollision, mLength))
		});
}
