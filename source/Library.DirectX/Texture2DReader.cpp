#include "pch.h"
#include "Texture2DReader.h"
#include "Game.h"
#include "GameException.h"
#include "StringHelper.h"
#include "TextureHelper.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace winrt;

namespace GameEngine
{
	RTTI_DEFINITIONS(Texture2DReader)

	Texture2DReader::Texture2DReader(Game& game) :
		ContentTypeReader(game, Texture2D::TypeIdClass())
	{
	}

	shared_ptr<Texture2D> Texture2DReader::_Read(const wstring& assetName)
	{
		com_ptr<ID3D11Resource> resource;
		com_ptr<ID3D11ShaderResourceView> shaderResourceView;
		if (StringHelper::EndsWith(assetName, L".dds"))
		{
			ThrowIfFailed(CreateDDSTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), resource.put(), shaderResourceView.put()), "CreateDDSTextureFromFile() failed.");
		}
		else
		{
			ThrowIfFailed(CreateWICTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), resource.put(), shaderResourceView.put()), "CreateWICTextureFromFile() failed.");
		}

		com_ptr<ID3D11Texture2D> texture = resource.as<ID3D11Texture2D>();
		Point textureSize = TextureHelper::GetTextureSize(not_null<ID3D11Texture2D*>(texture.get()));

		return make_shared<Texture2D>(move(shaderResourceView), textureSize.X, textureSize.Y);
	}
}