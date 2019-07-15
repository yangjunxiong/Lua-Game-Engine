#pragma once

#include "Entity.h"
#include <DirectXTK\Mouse.h>
#include <memory>
#include <windows.h>

namespace DirectX
{
	class Mouse;
}

namespace GameEngine
{
	class Game;

	enum class MouseButtons
	{
		Left = 0,
		Right,
		Middle,
		X1,
		X2
	};

	enum class MouseModes
	{
		Absolute = 0,
		Relative
	};

	class MouseEntity final : public Entity
	{
		RTTI_DECLARATIONS(MouseEntity, Entity)

	public:
		static DirectX::Mouse* Mouse();
		
		explicit MouseEntity(MouseModes mode = MouseModes::Relative);
		MouseEntity(const MouseEntity&) = delete;
		MouseEntity(MouseEntity&&) = default;
		MouseEntity& operator=(const MouseEntity&) = delete;
		MouseEntity& operator=(MouseEntity&&) = default;
		~MouseEntity() = default;

		const DirectX::Mouse::State& CurrentState() const;
		const DirectX::Mouse::State& LastState() const;

		virtual void Start(WorldState&) override;
		virtual void Update(WorldState&) override;
		void SetWindow(HWND window);

		int X() const;
		int Y() const;
		int Wheel() const;

		bool IsButtonUp(MouseButtons button) const;
		bool IsButtonDown(MouseButtons button) const;
		bool WasButtonUp(MouseButtons button) const;
		bool WasButtonDown(MouseButtons button) const;
		bool WasButtonPressedThisFrame(MouseButtons button) const;
		bool WasButtonReleasedThisFrame(MouseButtons button) const;
		bool IsButtonHeldDown(MouseButtons Button) const;

		MouseModes Mode() const;
		void SetMode(MouseModes mode);

	private:
		bool GetButtonState(const DirectX::Mouse::State& state, MouseButtons button) const;

		Game* mGame = nullptr;
		static std::unique_ptr<DirectX::Mouse> sMouse;

		DirectX::Mouse::State mCurrentState;
		DirectX::Mouse::State mLastState;
	};
}