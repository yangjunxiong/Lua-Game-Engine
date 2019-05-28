#include "pch.h"
#include "HealthUI.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "ActionRender.h"
#include "WorldState.h"

using namespace GameEngine;
using namespace std;
using namespace glm;
RTTI_DEFINITIONS(HealthUI);

HealthUI::HealthUI() :
	Entity(HealthUI::TypeIdClass())
{
	Event<EventMessageAttributed>::Subscribe(*this);
}

HealthUI::~HealthUI()
{
	Event<EventMessageAttributed>::Unsubscribe(*this);
}

void HealthUI::Start(WorldState& state)
{
	Entity::Start(state);

	int texId = mPlayerId == 0 ? 3 : 4;
	for (size_t i = 0; i < size_t(mHealthPoint); ++i)
	{
		ActionRender& render = static_cast<ActionRender&>(*CreateAction("ActionRender", "Render"));
		render.SetTexture(texId);
		render.SetSprite(0, 0);
		render.SetOffset(Transform(vec4(1 * float(i), 0, 0, 1), vec4(0), vec4(1)));
		mRenders.PushBack(&render);
	}
}

void HealthUI::Notify(const BaseEvent& event)
{
	const EventMessageAttributed& message = static_cast<const Event<EventMessageAttributed>&>(event).Message();
	if (message["Subtype"] == "PlayerDamage")
	{
		int id = message["PlayerId"].AsInt();
		if (id == mPlayerId)
		{
			int newHealth = message["NewHealth"].AsInt();
			mHealthPoint = newHealth;
			while (mRenders.Size() > size_t(newHealth))
			{
				message.GetWorldState()->GetWorld()->Destroy(*mRenders.Back());
				mRenders.PopBack();
			}
		}
	}
}

gsl::owner<Scope*> HealthUI::Clone() const
{
	return new HealthUI(*this);
}

const Vector<Attributed::Signature> HealthUI::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Health", Datum::DatumType::Integer, 1, offsetof(HealthUI, mHealthPoint)),
		Signature("PlayerId", Datum::DatumType::Integer, 1, offsetof(HealthUI, mPlayerId))
	});
}
