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

	ENUM();
	enum class MouseButtons
	{
		Left = 0,
		Right,
		Middle,
		X1,
		X2,

		Begin = Left,
		End = X2
	};

	enum class MouseModes
	{
		Absolute = 0,
		Relative
	};

	ENUM();
	enum class MouseEventType
	{
		Press = 0,
		Release = 1,
		Click = 2,
		Wheel = 3,

		Begin = Press,
		End = Wheel
	};

	CLASS();
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

		FUNCTION();
		int X() const;

		FUNCTION();
		int Y() const;

		FUNCTION();
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
		inline bool GetButtonState(const DirectX::Mouse::State& state, MouseButtons button) const;
		inline void UpdateMouseEvent(WorldState& state);
		inline void SendMouseEvent(WorldState& state, MouseEventType type, MouseButtons button);

		Game* mGame = nullptr;
		static std::unique_ptr<DirectX::Mouse> sMouse;

		DirectX::Mouse::State mCurrentState;
		DirectX::Mouse::State mLastState;
		std::vector<float> mMousePressTime;
		static const std::vector<bool(*)(const DirectX::Mouse::State&)> sButtonGetters;
		static const inline float sClickThreshold = 0.2f;
	};

	CLASS(EventMessage);
	class MouseEvent final
	{
	public:
		PROPERTY();
		MouseEventType Type;

		PROPERTY();
		MouseButtons Button;
	};
}