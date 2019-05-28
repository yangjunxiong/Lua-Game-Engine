#include "pch.h"
#include "EventMessageAttributed.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(EventMessageAttributed);

EventMessageAttributed::EventMessageAttributed() :
	Attributed(EventMessageAttributed::TypeIdClass())
{}

EventMessageAttributed::EventMessageAttributed(const std::string& subtype, WorldState& state) :
	Attributed(EventMessageAttributed::TypeIdClass()),
	mSubtype(subtype),
	mWorldState(&state)
{}

gsl::owner<Scope*> EventMessageAttributed::Clone() const
{
	return new EventMessageAttributed(*this);
}

const std::string& EventMessageAttributed::GetSubtype() const
{
	return mSubtype;
}

WorldState* EventMessageAttributed::GetWorldState() const
{
	return mWorldState;
}

void EventMessageAttributed::SetSubtype(const std::string& subtype)
{
	mSubtype = subtype;
}

void EventMessageAttributed::SetWorldState(WorldState& state)
{
	mWorldState = &state;
}

const Vector<Attributed::Signature> EventMessageAttributed::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Subtype", Datum::DatumType::String, 1, offsetof(EventMessageAttributed, mSubtype)),
		Signature("WorldState", Datum::DatumType::Pointer, 1, offsetof(EventMessageAttributed, mWorldState))
	});
}