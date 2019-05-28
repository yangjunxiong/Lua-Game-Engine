#pragma once
#include "Entity.h"
#include "Factory.h"

namespace UnitTestLibraryDesktop
{
	class Avatar : public GameEngine::Entity
	{
		RTTI_DECLARATIONS(Avatar, GameEngine::Entity);

	public:
		explicit Avatar(GameEngine::RTTI::IdType type = Avatar::TypeIdClass());
		virtual gsl::owner<GameEngine::Scope*> Clone() const override;
		virtual void Update(GameEngine::WorldState& state) override;

		static const GameEngine::Vector<GameEngine::Attributed::Signature> Signatures();
		int mValue = 0;
		size_t mTickCount = 0;
		int64_t mGameTime = 0;
		int64_t mFrameTime = 0;
	};

	DECLARE_FACTORY(Avatar, GameEngine::Scope);
}