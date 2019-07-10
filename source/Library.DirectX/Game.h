#pragma once

#include <windows.h>
#include <winrt\Windows.Foundation.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <memory>
#include <functional>
#include <array>

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "GameClock.h"
#include "GameTime.h"
#include "ServiceContainer.h"
#include "RenderTarget.h"
#include "ContentManager.h"
#include "LuaBind.h"
#include "EventQueue.h"

namespace GameEngine
{
	class GameComponent;

	class IDeviceNotify
	{
	public:
		virtual ~IDeviceNotify() { };

		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

	protected:
		IDeviceNotify() { };
	};

	CLASS();
    class Game : public RenderTarget
    {
		RTTI_DECLARATIONS(Game, RenderTarget);

    public:
        Game(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;
		virtual ~Game() = default;

		gsl::not_null<ID3D11Device5*> Direct3DDevice() const;
		gsl::not_null<ID3D11DeviceContext4*> Direct3DDeviceContext() const;
		gsl::not_null<IDXGISwapChain1*> SwapChain() const;
		gsl::not_null<ID3D11RenderTargetView*> RenderTargetView() const;
		gsl::not_null<ID3D11DepthStencilView*> DepthStencilView() const;
		SIZE RenderTargetSize() const;
		float AspectRatio() const;
		bool IsFullScreen() const;
		const D3D11_TEXTURE2D_DESC& BackBufferDesc() const;
		const D3D11_VIEWPORT& Viewport() const;
		std::uint32_t MultiSamplingCount() const;
		std::uint32_t MultiSamplingQualityLevels() const;
		std::shared_ptr<GameEngine::Lua::LuaBind> Lua() const;
		GameEngine::EventQueue& EventQueue();

		const std::vector<std::shared_ptr<GameComponent>>& Components() const;
		const ServiceContainer& Services() const;			

        virtual void Initialize();
		virtual void Run();
		virtual void Shutdown();  

		virtual void Update(const GameEngine::GameTime& gameTime);
		virtual void Draw(const GameEngine::GameTime& gameTime);

		void UpdateRenderTargetSize();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
		
		std::function<void*()> GetWindowCallback() const;

		ContentManager& Content();

    protected:		
		virtual void HandleDeviceLost();

		virtual void Begin() override;
		virtual void End() override;

		virtual void CreateDeviceIndependentResources();
		virtual void CreateDeviceResources();
		virtual void CreateWindowSizeDependentResources();

		inline static const D3D_FEATURE_LEVEL DefaultFeatureLevel{ D3D_FEATURE_LEVEL_9_1 };
		inline static const std::uint32_t DefaultFrameRate{ 60 };
		inline static const std::uint32_t DefaultMultiSamplingCount{ 4 };
		inline static const std::uint32_t DefaultBufferCount{ 2 };

		winrt::com_ptr<ID3D11Device5> mDirect3DDevice;
		winrt::com_ptr<ID3D11DeviceContext4> mDirect3DDeviceContext;
		winrt::com_ptr<IDXGISwapChain1> mSwapChain;
		D3D_FEATURE_LEVEL mFeatureLevel = DefaultFeatureLevel;

		D3D11_TEXTURE2D_DESC mBackBufferDesc;
		winrt::com_ptr<ID3D11RenderTargetView> mRenderTargetView;
		winrt::com_ptr<ID3D11DepthStencilView> mDepthStencilView;
		D3D11_VIEWPORT mViewport;

		std::uint32_t mFrameRate{ DefaultFrameRate };
		bool mIsFullScreen{ false };
		std::uint32_t mMultiSamplingCount{ DefaultMultiSamplingCount };
		std::uint32_t mMultiSamplingQualityLevels{ 0 };

		std::function<void*()> mGetWindow;
		std::function<void(SIZE&)> mGetRenderTargetSize;
		SIZE mRenderTargetSize;
		IDeviceNotify* mDeviceNotify;

		GameEngine::GameClock mGameClock;
        GameEngine::GameTime mGameTime;
		std::vector<std::shared_ptr<GameComponent>> mComponents;
		ServiceContainer mServices;
		ContentManager mContentManager;
		GameEngine::EventQueue mEventQueue;
		std::shared_ptr<GameEngine::Lua::LuaBind> mLua;
    };
}

#include "Game.inl"