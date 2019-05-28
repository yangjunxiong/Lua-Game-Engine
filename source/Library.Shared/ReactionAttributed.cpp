#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ReactionAttributed);

ReactionAttributed::ReactionAttributed(RTTI::IdType type) :
	Reaction(type)
{
	Event<EventMessageAttributed>::Subscribe(*this);
}

ReactionAttributed::ReactionAttributed(const ReactionAttributed& other) :
	Reaction(other),
	mSubtype(other.mSubtype)
{
	Event<EventMessageAttributed>::Subscribe(*this);
}

ReactionAttributed::ReactionAttributed(ReactionAttributed&& other) :
	Reaction(move(other)),
	mSubtype(move(other.mSubtype))
{
	Event<EventMessageAttributed>::Subscribe(*this);
}

ReactionAttributed::~ReactionAttributed()
{
	Event<EventMessageAttributed>::Unsubscribe(*this);
}

void ReactionAttributed::SetSubtype(const std::string& type)
{
	mSubtype = type;
}

const std::string& ReactionAttributed::GetSubtype() const
{
	return mSubtype;
}

gsl::owner<Scope*> ReactionAttributed::Clone() const
{
	return new ReactionAttributed(*this);
}

void ReactionAttributed::Notify(const BaseEvent& event)
{
	if (event.Is(Event<EventMessageAttributed>::TypeIdClass()))
	{
		auto& message = static_cast<const Event<EventMessageAttributed>&>(event).Message();
		if (message.GetSubtype() == mSubtype)
		{
			// Copy parameters
			auto parameters = message.AuxiliaryAttributes();
			for (const auto& pair : parameters)
			{
				AppendAuxiliaryAttribute(pair->first) = pair->second;
			}

			// Invoke action list
			WorldState* state = message.GetWorldState();
			assert(state != nullptr);
			ActionList::Update(*state);

			// Delete parameters
			for (const auto& pair : parameters)
			{
				RemoveAuxiliaryAttribute(pair->first);
			}
		}
	}
}

const Vector<Attributed::Signature> ReactionAttributed::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Subtype", Datum::DatumType::String, 1, offsetof(ReactionAttributed, mSubtype))
	});
}