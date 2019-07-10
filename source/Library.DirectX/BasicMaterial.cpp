#include "pch.h"
#include "BasicMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace GameEngine
{
	RTTI_DEFINITIONS(BasicMaterial)

	BasicMaterial::BasicMaterial(Game& game) :
		Material(game)
	{
	}	

	void BasicMaterial::SetSurfaceColor(const DirectX::XMFLOAT4& color)
	{
		SetSurfaceColor(reinterpret_cast<const float*>(&color));
	}

	uint32_t BasicMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void BasicMaterial::Initialize()
	{
		Material::Initialize();

		auto& content = mGame->Content();
		auto vertexShader = content.Load<VertexShader>(L"Shaders\\BasicVS.cso");
		SetShader(vertexShader);

		auto pixelShader = content.Load<PixelShader>(L"Shaders\\BasicPS.cso");
		SetShader(pixelShader);

		auto direct3DDevice = mGame->Direct3DDevice();
		vertexShader->CreateInputLayout<VertexPosition>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVSConstantBuffer.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVSConstantBuffer.get());

		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4);		
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPSConstantBuffer.put()), "ID3D11Device::CreateBuffer() failed.");
		AddConstantBuffer(ShaderStages::PS, mPSConstantBuffer.get());

		SetSurfaceColor(Colors::White.f);
	}

	void BasicMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVSConstantBuffer.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}

	void BasicMaterial::SetSurfaceColor(const float* color)
	{
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPSConstantBuffer.get(), 0, nullptr, color, 0, 0);
	}
}