#include "pch.h"
#include "PixelShader.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace GameEngine
{
	RTTI_DEFINITIONS(PixelShader)

	PixelShader::PixelShader(const com_ptr<ID3D11PixelShader>& vertexShader) :
		mShader(vertexShader)
	{
	}

	com_ptr<ID3D11PixelShader> PixelShader::Shader() const
	{
		return mShader;
	}
}