#include "pch.h"
#include "Obstacle.h"

using namespace GameEngine;
RTTI_DEFINITIONS(Obstacle)

Obstacle::Obstacle(RTTI::IdType type) : Entity(type)
{
	// Create the collision action and put it into this
}

Obstacle::~Obstacle()
{
}
