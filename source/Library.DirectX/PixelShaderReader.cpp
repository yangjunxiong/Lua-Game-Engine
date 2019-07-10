#include "pch.h"
#include "PixelShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace GameEngine
{
	RTTI_DEFINITIONS(PixelShaderReader)

	PixelShaderReader::PixelShaderReader(Game& game) :
		ContentTypeReader(game, PixelShader::TypeIdClass())
	{
	}

	shared_ptr<PixelShader> PixelShaderReader::_Read(const wstring& assetName)
	{
		com_ptr<ID3D11PixelShader> pixelShader;
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(assetName, compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(compiledPixelShader.data(), compiledPixelShader.size(), nullptr, pixelShader.put()), "ID3D11Device::CreatedPixelShader() failed.");
		
		return shared_ptr<PixelShader>(new PixelShader(move(pixelShader)));
	}

	PixelShaderWithClassLinkageReader::PixelShaderWithClassLinkageReader(Game& game, com_ptr<ID3D11ClassLinkage> classLinkage) :
		mGame(&game), mClassLinkage(classLinkage)
	{
	}

	shared_ptr<PixelShader> PixelShaderWithClassLinkageReader::operator()(const wstring& assetName) const
	{
		assert(mClassLinkage != nullptr);

		com_ptr<ID3D11PixelShader> pixelShader;
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(assetName, compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(compiledPixelShader.data(), compiledPixelShader.size(), mClassLinkage.get(), pixelShader.put()), "ID3D11Device::CreatedPixelShader() failed.");

		return shared_ptr<PixelShader>(new PixelShader(move(pixelShader)));
	}
}