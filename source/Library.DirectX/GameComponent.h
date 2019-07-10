#pragma once

#include "RTTI.h"
#include <gsl\gsl>

namespace GameEngine
{
	class GameTime;
}

namespace GameEngine
{
	class Game;

	CLASS();
	class GameComponent : public RTTI
	{
		RTTI_DECLARATIONS(GameComponent, RTTI)

	public:
		GameComponent() = default;
		GameComponent(Game& game);
		GameComponent(const GameComponent&) = default;
		GameComponent& operator=(const GameComponent&) = default;
		GameComponent(GameComponent&&) = default;
		GameComponent& operator=(GameComponent&&) = default;
		virtual ~GameComponent() = default;

		FUNCTION();
		Game* GetGame();

		FUNCTION();
		void SetGame(Game& game);

		FUNCTION();
		bool Enabled() const;

		FUNCTION();
		void SetEnabled(bool enabled);

		virtual void Initialize();
		virtual void Shutdown();

		FUNCTION();
		virtual void Update(const GameEngine::GameTime& gameTime);

	protected:
		gsl::not_null<Game*> mGame;
		bool mEnabled{ true };
	};
}