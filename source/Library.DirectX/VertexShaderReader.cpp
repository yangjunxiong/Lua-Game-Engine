#include "pch.h"
#include "VertexShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace GameEngine
{
	RTTI_DEFINITIONS(VertexShaderReader)

	VertexShaderReader::VertexShaderReader(Game& game) :
		ContentTypeReader(game, VertexShader::TypeIdClass())
	{
	}

	shared_ptr<VertexShader> VertexShaderReader::_Read(const wstring& assetName)
	{
		com_ptr<ID3D11VertexShader> vertexShader;
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(assetName, compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(compiledVertexShader.data(), compiledVertexShader.size(), nullptr, vertexShader.put()), "ID3D11Device::CreatedVertexShader() failed.");
		
		return shared_ptr<VertexShader>(new VertexShader(move(compiledVertexShader), move(vertexShader)));
	}
}