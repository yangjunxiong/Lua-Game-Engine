#include "pch.h"
#include "ActionAnimation.h"
#include "WorldState.h"
#include "GameTime.h"
#include "Entity.h"
#include "ActionRender.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionAnimation);
RTTI_DEFINITIONS(AnimState);
RTTI_DEFINITIONS(AnimClip);

#pragma region ActionAnimation
ActionAnimation::ActionAnimation() :
	Action(ActionAnimation::TypeIdClass())
{
	assert(mDatumPointers[STATE_INDEX]->first == STATE_KEY);
}

Datum& ActionAnimation::GetStates()
{
	return mDatumPointers[STATE_INDEX]->second;
}

void ActionAnimation::Start(WorldState& state)
{
	Action::Start(state);
	ChangeState(0);
}

void ActionAnimation::Update(WorldState& ws)
{
	Action::Update(ws);

	Datum& states = GetStates();
	assert(states.Size() > size_t(mActiveState));
	Scope& scope = states.AsTable(mActiveState);
	assert(scope.Is(AnimState::TypeIdClass()));
	AnimState& state = static_cast<AnimState&>(scope);
	state.Update(ws);
}

void ActionAnimation::SetAnimState(int id)
{
	Datum& states = GetStates();
	if (states.Size() > size_t(id))
	{
		ChangeState(id);
	}
}

void ActionAnimation::SetAnimState(const std::string& name)
{
	Datum& states = GetStates();
	for (size_t i = 0; i < states.Size(); ++i)
	{
		assert(states[int(i)].Is(AnimState::TypeIdClass()));
		AnimState& state = static_cast<AnimState&>(states[int(i)]);
		if (state.mName == name)
		{
			ChangeState(int(i));
			break;
		}
	}
}

gsl::owner<ActionAnimation*> ActionAnimation::Clone() const
{
	return new ActionAnimation(*this);
}

const Vector<Attributed::Signature> ActionAnimation::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature(STATE_KEY, Datum::DatumType::Table, 0)
	});
}

void ActionAnimation::ChangeState(int id)
{
	Datum& states = GetStates();
	AnimState& oldState = static_cast<AnimState&>(states[mActiveState]);
	oldState.Leave();

	mActiveState = id;
	AnimState newState = static_cast<AnimState&>(states[mActiveState]);
	newState.Enter();
}
#pragma endregion

#pragma region AnimState
AnimState::AnimState() :
	Attributed(AnimState::TypeIdClass())
{
	assert(mDatumPointers[CLIP_INDEX]->first == CLIP_KEY);
}

void AnimState::Update(WorldState& state)
{
	Datum& clips = GetAnimClips();
	assert(clips.Size() > size_t(mActiveClip));
	Scope& scope = clips.AsTable(mActiveClip);
	assert(scope.Is(AnimClip::TypeIdClass()));

	AnimClip& clip = static_cast<AnimClip&>(scope);
	auto deltaTime = state.GetGameTime().ElapsedGameTime().count();
	mTimeSinceLastClip += int(deltaTime);
	if (mTimeSinceLastClip > clip.mDuration)
	{
		if (size_t(mActiveClip) == clips.Size() - 1)
		{
			if (mLoop)
			{
				mActiveClip = 0;
				mTimeSinceLastClip = 0;
				UpdateSprite(static_cast<AnimClip&>(clips[mActiveClip]));
			}
		}
		else
		{
			++mActiveClip;
			mTimeSinceLastClip = 0;
			UpdateSprite(static_cast<AnimClip&>(clips[mActiveClip]));
		}
	}
}

void AnimState::UpdateSprite(AnimClip& clip)
{
	auto [actions, s] = Search(Entity::ACTION_TABLE_KEY);
	if (actions != nullptr)
	{
		for (size_t i = 0; i < actions->Size(); ++i)
		{
			Scope& scope = actions->AsTable(i);
			assert(scope.Is(Action::TypeIdClass()));
			if (scope.Is(ActionRender::TypeIdClass()))
			{
				ActionRender& render = static_cast<ActionRender&>(scope);
				render.SetTexture(clip.mTextureId);
				render.SetSprite(clip.mRow, clip.mColumn);
				render.SetHorizontalFlip(clip.mHorizontalFlip);
				render.SetVerticalFlip(clip.mVerticalFlip);
			}
		}
	}
}

Datum& AnimState::GetAnimClips()
{
	return mDatumPointers[CLIP_INDEX]->second;
}

void AnimState::Leave()
{
	mActiveClip = 0;
	mTimeSinceLastClip = 0L;
}

void AnimState::Enter()
{
	mActiveClip = 0;
	mTimeSinceLastClip = 0;
	Datum& clips = GetAnimClips();
	AnimClip& clip = static_cast<AnimClip&>(clips[mActiveClip]);
	UpdateSprite(clip);
}

gsl::owner<AnimState*> AnimState::Clone() const
{
	return new AnimState(*this);
}

const Vector<Attributed::Signature> AnimState::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature(CLIP_KEY, Datum::DatumType::Table, 0),
		Signature("Name", Datum::DatumType::String, 1, offsetof(AnimState, mName)),
		Signature("Loop", Datum::DatumType::Integer, 1, offsetof(AnimState, mLoop))
	});
}

AnimClip::AnimClip() :
	Attributed(AnimClip::TypeIdClass())
{}

const Vector<Attributed::Signature> AnimClip::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Texture", Datum::DatumType::Integer, 1, offsetof(AnimClip, mTextureId)),
		Signature("Row", Datum::DatumType::Integer, 1, offsetof(AnimClip, mRow)),
		Signature("Column", Datum::DatumType::Integer, 1, offsetof(AnimClip, mColumn)),
		Signature("HorizontalFlip", Datum::DatumType::Integer, 1, offsetof(AnimClip, mHorizontalFlip)),
		Signature("VerticalFlip", Datum::DatumType::Integer, 1, offsetof(AnimClip, mVerticalFlip)),
		Signature("Duration", Datum::DatumType::Integer, 1, offsetof(AnimClip, mDuration))
	});
}
#pragma endregion
