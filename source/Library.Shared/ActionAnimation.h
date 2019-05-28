#pragma once
#include "Action.h"

namespace GameEngine
{
	class AnimState;
	class AnimClip;

	class ActionAnimation final : public Action
	{
		RTTI_DECLARATIONS(ActionAnimation, Action);

	public:
		ActionAnimation();
		virtual void Start(WorldState& state) override;
		virtual void Update(WorldState& state) override;
		virtual gsl::owner<ActionAnimation*> Clone() const override;
		Datum& GetStates();
		void SetAnimState(int id);
		void SetAnimState(const std::string& name);

		static const Vector<Attributed::Signature> Signatures();
		static const inline std::string STATE_KEY = "State";
		static const inline size_t STATE_INDEX = 2;

	private:
		void ChangeState(int id);

		int mActiveState = 0;
	};

	class AnimState : public Attributed
	{
		RTTI_DECLARATIONS(AnimState, Attributed);

	public:
		AnimState();

		void Update(WorldState& state);
		void Leave();
		void Enter();
		Datum& GetAnimClips();
		virtual gsl::owner<AnimState*> Clone() const override;
		static const Vector<Attributed::Signature> Signatures();
		static const inline std::string CLIP_KEY = "Clip";
		static const inline size_t CLIP_INDEX = 1;

		std::string mName;
		bool mLoop = true;
		int mActiveClip = 0;
		int mTimeSinceLastClip = 0;

	private:
		void UpdateSprite(AnimClip& clip);
	};

	class AnimClip : public Attributed
	{
		RTTI_DECLARATIONS(AnimClip, Attributed);

	public:
		AnimClip();
		static const Vector<Attributed::Signature> Signatures();

		int mTextureId = 0;
		int mRow = 0;
		int mColumn = 0;
		int mHorizontalFlip = 0;
		int mVerticalFlip = 0;
		int mDuration = 0;
	};

	DECLARE_FACTORY(ActionAnimation, Scope);
	DECLARE_FACTORY(AnimState, Scope);
	DECLARE_FACTORY(AnimClip, Scope);
}