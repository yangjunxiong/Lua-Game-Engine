#include "pch.h"
#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "SpotLight.h"
#include "PlanetMaterial.h"
#include "Game.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexDeclarations.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace GameEngine;
using namespace DirectX;
RTTI_DEFINITIONS(PlanetMaterial);

PlanetMaterial::PlanetMaterial(Game& game) :
	Material(game)
{}

com_ptr<ID3D11SamplerState> PlanetMaterial::SamplerState() const
{
	return mSamplerState;
}

void PlanetMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
{
	assert(samplerState != nullptr);
	mSamplerState = samplerState;
	Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
}

shared_ptr<Texture2D> PlanetMaterial::ColorMap() const
{
	return mColorMap;
}

void PlanetMaterial::SetColorMap(shared_ptr<Texture2D> texture)
{
	assert(texture != nullptr);
	mColorMap = move(texture);
	ResetPixelShaderResources();
}

shared_ptr<Texture2D> PlanetMaterial::SpecularMap() const
{
	return mSpecularMap;
}

void PlanetMaterial::SetSpecularMap(shared_ptr<Texture2D> texture)
{
	assert(texture != nullptr);
	mSpecularMap = move(texture);
	ResetPixelShaderResources();
}

const XMFLOAT4& PlanetMaterial::AmbientColor() const
{
	return mPixelCBufferPerFrameData.AmbientColor;
}

void PlanetMaterial::SetAmbientColor(const XMFLOAT4& color)
{
	mPixelCBufferPerFrameData.AmbientColor = color;
	mPixelCBufferPerFrameDataDirty = true;
}

const XMFLOAT3& PlanetMaterial::LightPosition() const
{
	return mPixelCBufferPerFrameData.LightPosition;
}

void PlanetMaterial::SetLightPosition(const XMFLOAT3& position)
{
	mPixelCBufferPerFrameData.LightPosition = position;
	mPixelCBufferPerFrameDataDirty = true;

	mVertexCBufferPerFrameData.LightPosition = position;
	mVertexCBufferPerFrameDataDirty = true;
}

float PlanetMaterial::LightRadius() const
{
	return mVertexCBufferPerFrameData.LightRadius;
}

void PlanetMaterial::SetLightRadius(float radius)
{
	mVertexCBufferPerFrameData.LightRadius = radius;
	mVertexCBufferPerFrameDataDirty = true;
}

const XMFLOAT4& PlanetMaterial::LightColor() const
{
	return mPixelCBufferPerFrameData.LightColor;
}

void PlanetMaterial::SetLightColor(const XMFLOAT4& color)
{
	mPixelCBufferPerFrameData.LightColor = color;
	mPixelCBufferPerFrameDataDirty = true;
}

const XMFLOAT3& PlanetMaterial::SpecularColor() const
{
	return mPixelCBufferPerObjectData.SpecularColor;
}

void PlanetMaterial::SetSpecularColor(const XMFLOAT3& color)
{
	mPixelCBufferPerObjectData.SpecularColor = color;
	mPixelCBufferPerObjectDataDirty = true;
}

float PlanetMaterial::SpecularPower() const
{
	return mPixelCBufferPerObjectData.SpecularPower;
}

void PlanetMaterial::SetSpecularPower(float power)
{
	mPixelCBufferPerObjectData.SpecularPower = power;
	mPixelCBufferPerObjectDataDirty = true;
}

uint32_t PlanetMaterial::VertexSize() const
{
	return sizeof(VertexPositionTextureNormal);
}

void PlanetMaterial::Initialize()
{
	Material::Initialize();

	// Load shaders
	auto& content = mGame->Content();
	auto vertexShader = content.Load<VertexShader>(L"Shaders\\PlanetVS.cso"s);
	SetShader(vertexShader);
	auto pixelShader = content.Load<PixelShader>(L"Shaders\\PlanetPS.cso");
	SetShader(pixelShader);

	// Create device and layout
	auto direct3DDevice = mGame->Direct3DDevice();
	vertexShader->CreateInputLayout<VertexPositionTextureNormal>(direct3DDevice);
	SetInputLayout(vertexShader->InputLayout());

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

void PlanetMaterial::UpdateCameraPosition(const XMFLOAT3& position)
{
	mPixelCBufferPerFrameData.CameraPosition = position;
	mPixelCBufferPerFrameDataDirty = true;
}

void PlanetMaterial::UpdateTransforms(FXMMATRIX worldViewProjectionMatrix, CXMMATRIX worldMatrix)
{
	XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
	XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);
	mGame->Direct3DDeviceContext()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
}

void PlanetMaterial::BeginDraw()
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

void PlanetMaterial::ResetPixelShaderResources()
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