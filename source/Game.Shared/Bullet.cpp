#include "pch.h"
#include "Bullet.h"
#include "GameTime.h"
#include "ActionRender.h"
#include "ActionBoxCollision.h"
#include "ReactionBullet.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(Bullet);

Bullet::Bullet() :
	Entity(Bullet::TypeIdClass())
{}

void Bullet::Update(WorldState& state)
{
	long long e = state.GetGameTime().ElapsedGameTime().count();
	float deltaTime = e / 1000.f;
	mTransform.SetPosition(mTransform.GetPosition() + mVelocity * deltaTime);

	Entity::Update(state);
}

gsl::owner<Bullet*> Bullet::Clone() const
{
	return new Bullet(*this);
}

const Vector<Attributed::Signature> Bullet::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Velocity", Datum::DatumType::Vector, 1, offsetof(Bullet, mVelocity))
	});
}

gsl::owner<Bullet*> Bullet::CreateDefaultBullet()
{
	Bullet* bullet = new Bullet();
	ActionRender& render = static_cast<ActionRender&>(*bullet->CreateAction("ActionRender", "Render"));
	render.SetTexture(1);
	render.SetSprite(1, 1);
	ActionBoxCollision& collision = static_cast<ActionBoxCollision&>(*bullet->CreateAction("ActionBoxCollision", "Render"));
	collision["Width"] = 0.1f;
	collision["Length"] = 0.1f;
	collision["IsStatic"] = 0;
	collision["IsSensor"] = 1;
	bullet->CreateAction("ReactionBullet", "Reaction");

	return bullet;
}