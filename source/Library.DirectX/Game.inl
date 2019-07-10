namespace GameEngine
{
	inline gsl::not_null<ID3D11Device5*> Game::Direct3DDevice() const
	{
		return gsl::not_null<ID3D11Device5*>(mDirect3DDevice.get());
	}

	inline gsl::not_null<ID3D11DeviceContext4*> Game::Direct3DDeviceContext() const
	{
		return gsl::not_null<ID3D11DeviceContext4*>(mDirect3DDeviceContext.get());
	}

	inline gsl::not_null<IDXGISwapChain1*> Game::SwapChain() const
	{
		return gsl::not_null<IDXGISwapChain1*>(mSwapChain.get());
	}

	inline gsl::not_null<ID3D11RenderTargetView*> Game::RenderTargetView() const
	{
		return gsl::not_null<ID3D11RenderTargetView*>(mRenderTargetView.get());
	}

	inline gsl::not_null<ID3D11DepthStencilView*> Game::DepthStencilView() const
	{
		return gsl::not_null<ID3D11DepthStencilView*>(mDepthStencilView.get());
	}

	inline SIZE Game::RenderTargetSize() const
	{
		return mRenderTargetSize;
	}

	inline float Game::AspectRatio() const
	{
		return static_cast<float>(mRenderTargetSize.cx) / mRenderTargetSize.cy;
	}

	inline bool Game::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	inline const D3D11_TEXTURE2D_DESC& Game::BackBufferDesc() const
	{
		return mBackBufferDesc;
	}

	inline const D3D11_VIEWPORT& Game::Viewport() const
	{
		return mViewport;
	}

	inline std::uint32_t Game::MultiSamplingCount() const
	{
		return mMultiSamplingCount;
	}

	inline std::uint32_t Game::MultiSamplingQualityLevels() const
	{
		return mMultiSamplingQualityLevels;
	}

	inline std::shared_ptr<GameEngine::Lua::LuaBind> Game::Lua() const
	{
		return mLua;
	}

	inline GameEngine::EventQueue& Game::EventQueue()
	{
		return mEventQueue;
	}

	inline const std::vector<std::shared_ptr<GameComponent>>& Game::Components() const
	{
		return mComponents;
	}

	inline const ServiceContainer& Game::Services() const
	{
		return mServices;
	}

	inline void Game::RegisterDeviceNotify(IDeviceNotify* deviceNotify)
	{
		mDeviceNotify = deviceNotify;
	}

	inline std::function<void*()> Game::GetWindowCallback() const
	{
		return mGetWindow;
	}

	inline ContentManager& Game::Content()
	{
		return mContentManager;
	}
}