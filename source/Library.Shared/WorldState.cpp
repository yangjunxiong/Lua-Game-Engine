#include "pch.h"
#include "WorldState.h"
#include "GameTime.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(WorldState);

WorldState::WorldState(const GameTime& time) :
	mTime(&time)
{}

const GameTime& WorldState::GetGameTime() const
{
	return *mTime;
}

World* WorldState::GetWorld()
{
	return mWorld;
}

Sector* WorldState::GetSector()
{
	return mSector;
}

Entity* WorldState::GetEntity()
{
	return mEntity;
}

Action* WorldState::GetAction()
{
	return mAction;
}