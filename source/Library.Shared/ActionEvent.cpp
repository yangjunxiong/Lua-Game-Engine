#include "pch.h"
#include "ActionEvent.h"
#include "World.h"
#include "Event.h"
#include "EventMessageAttributed.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionEvent);

ActionEvent::ActionEvent(RTTI::IdType type) :
	Action(type)
{}

void ActionEvent::Update(class WorldState& state)
{
	// Create the event
	EventMessageAttributed payload(mSubtype, state);
	auto parameters = AuxiliaryAttributes();
	for (const auto& pair : parameters)
	{
		payload.AppendAuxiliaryAttribute(pair->first) = pair->second;
	}
	shared_ptr<Event<EventMessageAttributed>> event = make_shared<Event<EventMessageAttributed>>(payload);

	// Enqueue the event
	World* world = state.GetWorld();
	assert(world != nullptr);
	world->EnqueueEvent(event, chrono::milliseconds(mDelay));
}

gsl::owner<Scope*> ActionEvent::Clone() const
{
	return new ActionEvent(*this);
}

const std::string& ActionEvent::GetSubtype() const
{
	return mSubtype;
}

const int& ActionEvent::GetDelay() const
{
	return mDelay;
}

void ActionEvent::SetSubtype(const std::string& subtype)
{
	mSubtype = subtype;
}

void ActionEvent::SetDelay(int delay)
{
	mDelay = delay;
}

const Vector<Attributed::Signature> ActionEvent::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Subtype", Datum::DatumType::String, 1, offsetof(ActionEvent, mSubtype)),
		Signature("Delay", Datum::DatumType::Integer, 1, offsetof(ActionEvent, mDelay))
	});
}
