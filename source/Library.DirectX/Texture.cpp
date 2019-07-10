#include "pch.h"
#include "Texture.h"

using namespace DirectX;
using namespace winrt;

namespace GameEngine
{
	RTTI_DEFINITIONS(Texture)

	Texture::Texture(const com_ptr<ID3D11ShaderResourceView>& shaderResourceView) :
		mShaderResourceView(shaderResourceView)
	{
	}
}