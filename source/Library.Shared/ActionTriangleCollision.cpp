#include "pch.h"
#include "ActionTriangleCollision.h"
#include "World.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "CollisionManager.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionTriangleCollision);

ActionTriangleCollision::ActionTriangleCollision(RTTI::IdType type) :
	ActionCollision(type)
{
	InitBody();
}

void ActionTriangleCollision::Update(class WorldState& state)
{
	if (!mIsInitialized)
	{
		mBody->DestroyFixture(mBody->GetFixtureList());
		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = mIsSensor;
		b2PolygonShape polygonShape;

		b2Vec2 vertices[3];
		vertices[0].Set(mVertices[0].x, mVertices[0].y);
		vertices[1].Set(mVertices[1].x, mVertices[1].y);
		vertices[2].Set(mVertices[2].x, mVertices[2].y);
		polygonShape.Set(vertices, 3);
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

void GameEngine::ActionTriangleCollision::InitBody()
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

gsl::owner<Scope*> ActionTriangleCollision::Clone() const
{
	return new ActionTriangleCollision(*this);
}

const Vector<Attributed::Signature> ActionTriangleCollision::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Vertices", Datum::DatumType::Vector, 3, offsetof(ActionTriangleCollision, mVertices))
		});
}
