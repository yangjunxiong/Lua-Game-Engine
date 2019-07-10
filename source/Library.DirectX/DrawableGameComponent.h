#pragma once

#include "TransformComponent.h"
#include <memory>

namespace GameEngine
{
	class GameTime;
}

namespace GameEngine
{
    class Camera;

	CLASS();
    class DrawableGameComponent : public TransformComponent
    {
        RTTI_DECLARATIONS(DrawableGameComponent, TransformComponent)

    public:
        DrawableGameComponent() = default;
		explicit DrawableGameComponent(Game& game, std::shared_ptr<Camera> camera = nullptr);
		DrawableGameComponent(const DrawableGameComponent&) = default;
		DrawableGameComponent& operator=(const DrawableGameComponent&) = default;
		DrawableGameComponent(DrawableGameComponent&&) = default;
		DrawableGameComponent& operator=(DrawableGameComponent&&) = default;
		virtual ~DrawableGameComponent() = default;		

		FUNCTION();
        bool Visible() const;

		FUNCTION();
        void SetVisible(bool visible);

		std::shared_ptr<Camera> GetCamera();
		void SetCamera(const std::shared_ptr<Camera>& camera);

        virtual void Draw(const GameEngine::GameTime& gameTime);

    protected:
		bool mVisible{ true };
		std::shared_ptr<Camera> mCamera;
    };
}