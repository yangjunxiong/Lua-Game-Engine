#include "pch.h"
#include "Avatar.h"
#include "WorldState.h"
#include "GameTime.h"

using namespace GameEngine;
using namespace UnitTestLibraryDesktop;

RTTI_DEFINITIONS(Avatar);

Avatar::Avatar(RTTI::IdType type) : Entity(type)
{}

gsl::owner<Scope*> Avatar::Clone() const
{
	return new Avatar(*this);
}

void Avatar::Update(WorldState& state)
{
	Entity::Update(state);
	++mTickCount;
	mGameTime = state.GetGameTime().TotalGameTime().count();
	mFrameTime = state.GetGameTime().ElapsedGameTime().count();
}

const Vector<Attributed::Signature> Avatar::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Value", Datum::DatumType::Integer, 1, offsetof(Avatar, mValue))
	});
}