#include "pch.h"
#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "MatrixHelper.h"
#include "SpotLight.h"
#include "StaticMeshMaterial.h"
#include "Game.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexDeclarations.h"
#include "Utility.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace GameEngine;
using namespace DirectX;
RTTI_DEFINITIONS(StaticMeshMaterial);

StaticMeshMaterial::StaticMeshMaterial() :
	Material(*Game::GetInstance())
{}

com_ptr<ID3D11SamplerState> StaticMeshMaterial::SamplerState() const
{
	return mSamplerState;
}

void StaticMeshMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
{
	assert(samplerState != nullptr);
	mSamplerState = samplerState;
	Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
}

shared_ptr<Texture2D> StaticMeshMaterial::ColorMap() const
{
	return mColorMap;
}

void StaticMeshMaterial::SetColorMap(shared_ptr<Texture2D> texture)
{
	assert(texture != nullptr);
	mColorMap = move(texture);
	ResetPixelShaderResources();
}

void StaticMeshMaterial::SetColorMap(const std::string& fileName)
{
	SetColorMap(mGame->Content().Load<Texture2D>(Utility::ToWideString(fileName)));
}

shared_ptr<Texture2D> StaticMeshMaterial::SpecularMap() const
{
	return mSpecularMap;
}

void StaticMeshMaterial::SetSpecularMap(shared_ptr<Texture2D> texture)
{
	assert(texture != nullptr);
	mSpecularMap = move(texture);
	ResetPixelShaderResources();
}

const XMFLOAT4& StaticMeshMaterial::AmbientColor() const
{
	return mPixelCBufferPerFrameData.AmbientColor;
}

void StaticMeshMaterial::SetAmbientColor(const XMFLOAT4& color)
{
	mPixelCBufferPerFrameData.AmbientColor = color;
	mPixelCBufferPerFrameDataDirty = true;
}

const XMFLOAT3& StaticMeshMaterial::LightPosition() const
{
	return mPixelCBufferPerFrameData.LightPosition;
}

void StaticMeshMaterial::SetLightPosition(const XMFLOAT3& position)
{
	mPixelCBufferPerFrameData.LightPosition = position;
	mPixelCBufferPerFrameDataDirty = true;

	mVertexCBufferPerFrameData.LightPosition = position;
	mVertexCBufferPerFrameDataDirty = true;
}

float StaticMeshMaterial::LightRadius() const
{
	return mVertexCBufferPerFrameData.LightRadius;
}

void StaticMeshMaterial::SetLightRadius(float radius)
{
	mVertexCBufferPerFrameData.LightRadius = radius;
	mVertexCBufferPerFrameDataDirty = true;
}

const XMFLOAT4& StaticMeshMaterial::LightColor() const
{
	return mPixelCBufferPerFrameData.LightColor;
}

void StaticMeshMaterial::SetLightColor(const XMFLOAT4& color)
{
	mPixelCBufferPerFrameData.LightColor = color;
	mPixelCBufferPerFrameDataDirty = true;
}

const XMFLOAT3& StaticMeshMaterial::SpecularColor() const
{
	return mPixelCBufferPerObjectData.SpecularColor;
}

void StaticMeshMaterial::SetSpecularColor(const XMFLOAT3& color)
{
	mPixelCBufferPerObjectData.SpecularColor = color;
	mPixelCBufferPerObjectDataDirty = true;
}

float StaticMeshMaterial::SpecularPower() const
{
	return mPixelCBufferPerObjectData.SpecularPower;
}

void StaticMeshMaterial::SetSpecularPower(float power)
{
	mPixelCBufferPerObjectData.SpecularPower = power;
	mPixelCBufferPerObjectDataDirty = true;
}

uint32_t StaticMeshMaterial::VertexSize() const
{
	return sizeof(VertexPositionTextureNormal);
}

void StaticMeshMaterial::Initialize()
{
	Material::Initialize();

	// Hard code some config
	SetLightPosition(XMFLOAT3(0.f, 0.f, 1000.f));
	SetLightColor(XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	SetLightRadius(10000.f);

	// Load shaders
	auto& content = mGame->Content();
	auto vertexShader = content.Load<VertexShader>(L"Shaders\\StaticMeshVS.cso"s);
	SetShader(vertexShader);
	auto pixelShader = content.Load<PixelShader>(L"Shaders\\StaticMeshPS.cso"s);
	SetShader(pixelShader);

	// Create device and layout
	auto direct3DDevice = mGame->Direct3DDevice();
	vertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
	SetInputLayout(vertexShader->InputLayout());
	SetSamplerState(SamplerStates::TrilinearWrap);

	// Bind shader constant buffers
	D3D11_BUFFER_DESC constantBufferDesc { 0 };
	constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerFrame);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");
	AddConstantBuffer(ShaderStages::VS, mVertexCBufferPerFrame.get());

	constantBufferDesc.ByteWidth = sizeof(VertexCBufferPerObject);
	ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
	AddConstantBuffer(ShaderStages::VS, mVertexCBufferPerObject.get());

	constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
	ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerFrame.put()), "ID3D11Device::CreateBuffer() failed.");
	AddConstantBuffer(ShaderStages::PS, mPixelCBufferPerFrame.get());

	constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerObject);
	ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerObject.put()), "ID3D11Device::CreateBuffer() failed.");
	AddConstantBuffer(ShaderStages::PS, mPixelCBufferPerObject.get());

	// Update shader constant buffer values
	auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
	direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
	direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
	direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
	direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);

	ResetPixelShaderResources();
	AddSamplerState(ShaderStages::PS, mSamplerState.get());
}

void StaticMeshMaterial::UpdateCameraPosition(const XMFLOAT3& position)
{
	mPixelCBufferPerFrameData.CameraPosition = position;
	mPixelCBufferPerFrameDataDirty = true;
}

void StaticMeshMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
{
	XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
	XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
	mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
}

void StaticMeshMaterial::BeginDraw()
{
	Material::BeginDraw();

	auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

	if (mVertexCBufferPerFrameDataDirty)
	{
		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerFrame.get(), 0, nullptr, &mVertexCBufferPerFrameData, 0, 0);
		mVertexCBufferPerFrameDataDirty = false;
	}

	if (mPixelCBufferPerFrameDataDirty)
	{
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerFrame.get(), 0, nullptr, &mPixelCBufferPerFrameData, 0, 0);
		mPixelCBufferPerFrameDataDirty = false;
	}

	if (mPixelCBufferPerObjectDataDirty)
	{
		direct3DDeviceContext->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
		mPixelCBufferPerObjectDataDirty = false;
	}
}

void StaticMeshMaterial::ResetPixelShaderResources()
{
	Material::ClearShaderResources(ShaderStages::PS);
	if (mSpecularMap == nullptr)
	{
		if (mColorMap != nullptr)
		{
			ID3D11ShaderResourceView* shaderResources[] = { mColorMap->ShaderResourceView().get() };
			Material::AddShaderResources(ShaderStages::PS, shaderResources);
		}
	}
	else
	{
		ID3D11ShaderResourceView* shaderResources[] = { mColorMap->ShaderResourceView().get(), mSpecularMap->ShaderResourceView().get() };
		Material::AddShaderResources(ShaderStages::PS, shaderResources);
	}
}