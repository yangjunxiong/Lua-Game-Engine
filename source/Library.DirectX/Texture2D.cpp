#include "pch.h"
#include "Texture2D.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace GameEngine
{
	RTTI_DEFINITIONS(Texture2D)

	Texture2D::Texture2D(const com_ptr<ID3D11ShaderResourceView>& shaderResourceView, const int32_t width, const int32_t height) :
		Texture(shaderResourceView), mWidth(width), mHeight(height)
	{
	}

	int32_t Texture2D::Width() const
	{
		return mWidth;
	}

	int32_t Texture2D::Height() const
	{
		return mHeight;
	}

	Rectangle Texture2D::Bounds() const
	{
		return Rectangle(0, 0, mWidth, mHeight);
	}	
}