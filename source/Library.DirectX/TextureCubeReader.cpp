#include "pch.h"
#include "TextureCubeReader.h"
#include "Game.h"
#include "GameException.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace GameEngine
{
	RTTI_DEFINITIONS(TextureCubeReader)

	TextureCubeReader::TextureCubeReader(Game& game) :
		ContentTypeReader(game, TextureCube::TypeIdClass())
	{
	}

	shared_ptr<TextureCube> TextureCubeReader::_Read(const wstring& assetName)
	{
		com_ptr<ID3D11ShaderResourceView> shaderResourceView;	
		ThrowIfFailed(CreateDDSTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), nullptr, shaderResourceView.put()), "CreateDDSTextureFromFile() failed.");

		return shared_ptr<TextureCube>(new TextureCube(move(shaderResourceView)));
	}
}