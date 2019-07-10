#pragma once
#include "Game.h"

namespace GameEngine
{
	class KeyboardComponent;
	class MouseComponent;
	class GamePadComponent;
	class FpsComponent;
	class Grid;
	class Skybox;
	class FirstPersonCamera;
	class VertexPositionColor;
}

namespace Rendering
{
	class SpotLightDemo;

	class RenderingGame final : public GameEngine::Game
	{
	public:
		RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const GameEngine::GameTime& gameTime) override;
		virtual void Draw(const GameEngine::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void Exit();

	private:
		inline static const DirectX::XMVECTORF32 BackgroundColor{ DirectX::Colors::CornflowerBlue };

		std::shared_ptr<GameEngine::KeyboardComponent> mKeyboard;
		std::shared_ptr<GameEngine::MouseComponent> mMouse;
		std::shared_ptr<GameEngine::GamePadComponent> mGamePad;
		std::shared_ptr<GameEngine::FpsComponent> mFpsComponent;
		std::shared_ptr<GameEngine::FirstPersonCamera> mCamera;
		std::shared_ptr<GameEngine::Grid> mGrid;
		std::shared_ptr<GameEngine::Skybox> mSkybox;
	};
}