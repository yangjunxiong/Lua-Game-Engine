#include "pch.h"
#include "Reaction.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(Reaction);

Reaction::Reaction(RTTI::IdType type) :
	ActionList(type)
{}

void Reaction::Update(WorldState&)
{}

const Vector<Attributed::Signature> Reaction::Signatures()
{
	return Vector<Attributed::Signature>();
}
