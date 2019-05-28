#include "pch.h"
#include "ActionCollision.h"
#include "CollisionManager.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionCollision);

#define RADTODEG 57.295779513082320876f

ActionCollision::ActionCollision(RTTI::IdType type) :
	Action(type)
{}

void GameEngine::ActionCollision::Update(WorldState & /*state*/)
{
	assert(mBody != nullptr);
	auto parent = GetParent();
	assert(parent->Is(Entity::TypeIdClass()));
	auto entity = static_cast<Entity*>(parent);

	glm::vec4 position = entity->GetPosition();
	glm::vec4 rotation = entity->GetRotation();
	mBody->SetTransform(b2Vec2(position.x, position.y), rotation.z / RADTODEG);
}

GameEngine::ActionCollision::~ActionCollision()
{
	CollisionManager::GetInstance().RemoveBody(*mBody);
}

const Vector<Attributed::Signature> ActionCollision::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("IsStatic", Datum::DatumType::Integer, 1, offsetof(ActionCollision, mIsStatic)),
		Signature("IsSensor", Datum::DatumType::Integer, 1, offsetof(ActionCollision, mIsSensor))
		});
}