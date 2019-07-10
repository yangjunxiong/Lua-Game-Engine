#include "pch.h"
#include "VertexShader.h"
#include "GameException.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace winrt;

namespace GameEngine
{
	RTTI_DEFINITIONS(VertexShader)

	VertexShader::VertexShader(const vector<char>& compiledShader, const com_ptr<ID3D11VertexShader>& vertexShader) :
		mCompiledShader(compiledShader), mShader(vertexShader)
	{
	}

	VertexShader::VertexShader(vector<char>&& compiledShader, const com_ptr<ID3D11VertexShader>& vertexShader) :
		mCompiledShader(compiledShader), mShader(vertexShader)
	{
	}

	const vector<char>& VertexShader::CompiledShader() const
	{
		return mCompiledShader;
	}

	com_ptr<ID3D11VertexShader> VertexShader::Shader() const
	{
		return mShader;
	}

	com_ptr<ID3D11InputLayout> VertexShader::InputLayout() const
	{
		return mInputLayout;
	}

	void VertexShader::CreateInputLayout(not_null<ID3D11Device*> device, const span<const D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions, bool releaseCompiledShader)
	{
		mInputLayout = nullptr;
		ThrowIfFailed(device->CreateInputLayout(inputElementDescriptions.data(), narrow_cast<uint32_t>(inputElementDescriptions.size()), mCompiledShader.data(), mCompiledShader.size(), mInputLayout.put()), "ID3D11Device::CreateInputLayout() failed.");

		if (releaseCompiledShader)
		{
			mCompiledShader.clear();
			mCompiledShader.shrink_to_fit();
		}
	}
}