#include "pch.h"
#include "ActionPolygonCollision.h"
#include "World.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "CollisionManager.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionPolygonCollision);

ActionPolygonCollision::ActionPolygonCollision(RTTI::IdType type) :
	ActionCollision(type)
{
	InitBody();
}

/*void ActionPolygonCollision::Update(class WorldState& state)
{
	assert(mBody != nullptr);

	
}

void GameEngine::ActionPolygonCollision::UpdateTransform()
{
	auto parent = GetParent();
	assert(parent->Is(Entity::TypeIdClass()));
	auto entity = static_cast<Entity*>(parent);

	glm::vec4 position = entity->GetPosition();
	glm::vec4 rotation = entity->GetRotation();
	mBody->SetTransform(b2Vec2(position.x, position.y), rotation.z);

	mBody->DestroyFixture(mBody->GetFixtureList());
	b2FixtureDef fixtureDef;
	b2PolygonShape polygonShape;
	b2Vec2 vertices3[3];
	b2Vec2 vertices4[4];

	switch (mType)
	{
	case PolygonType::Triangle:
		vertices3[0].Set(mVertices[0].x, mVertices[0].y);
		vertices3[1].Set(mVertices[1].x, mVertices[1].y);
		vertices3[2].Set(mVertices[2].x, mVertices[2].y);
		polygonShape.Set(vertices3, 3);

		break;
	case PolygonType::Box:
		vertices4[0].Set(mVertices[0].x, mVertices[0].y);
		vertices4[1].Set(mVertices[1].x, mVertices[1].y);
		vertices4[2].Set(mVertices[2].x, mVertices[2].y);
		vertices4[3].Set(mVertices[3].x, mVertices[3].y);
		polygonShape.Set(vertices4, 4);

		break;
	}

	fixtureDef.shape = &polygonShape;
	mBody->CreateFixture(&fixtureDef);
}*/

void GameEngine::ActionPolygonCollision::InitBody()
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
	mBody->SetUserData(GetParent());
}

gsl::owner<Scope*> ActionPolygonCollision::Clone() const
{
	return new ActionPolygonCollision(*this);
}

const Vector<Attributed::Signature> ActionPolygonCollision::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Type", Datum::DatumType::Integer, 1, offsetof(ActionPolygonCollision, mType)),
		Signature("Vertices", Datum::DatumType::Vector, 4, offsetof(ActionPolygonCollision, mVertices))
		});
}
