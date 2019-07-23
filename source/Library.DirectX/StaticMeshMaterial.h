#pragma once
#include <DirectXColors.h>
#include "Material.h"
#include "Texture2D.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "SamplerStates.h"
#include "Vector4.h"

namespace GameEngine
{
	CLASS();
	class StaticMeshMaterial : public Material
	{
		RTTI_DECLARATIONS(StaticMeshMaterial, Material);

	public:
		StaticMeshMaterial();
		StaticMeshMaterial(const StaticMeshMaterial&) = default;
		StaticMeshMaterial& operator=(const StaticMeshMaterial&) = default;
		StaticMeshMaterial(StaticMeshMaterial&&) = default;
		StaticMeshMaterial& operator=(StaticMeshMaterial&&) = default;
		virtual ~StaticMeshMaterial() = default;

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(winrt::com_ptr<ID3D11SamplerState> samplerState);

		std::shared_ptr<GameEngine::Texture2D> ColorMap() const;
		void SetColorMap(std::shared_ptr<GameEngine::Texture2D> texture);
		FUNCTION();
		void SetColorMap(const std::string& fileName);

		std::shared_ptr<GameEngine::Texture2D> SpecularMap() const;
		void SetSpecularMap(std::shared_ptr<GameEngine::Texture2D> texture);

		const DirectX::XMFLOAT4& AmbientColor() const;
		void SetAmbientColor(const DirectX::XMFLOAT4& color);

		const DirectX::XMFLOAT3& LightPosition() const;
		void SetLightPosition(const DirectX::XMFLOAT3& position);

		float LightRadius() const;
		void SetLightRadius(float radius);

		const DirectX::XMFLOAT4& LightColor() const;
		void SetLightColor(const DirectX::XMFLOAT4& color);

		const DirectX::XMFLOAT3& SpecularColor() const;
		void SetSpecularColor(const DirectX::XMFLOAT3& color);

		FUNCTION();
		Vector3 CoverColor() const;

		FUNCTION();
		void SetCoverColor(const Vector3& color);

		FUNCTION();
		float Outline() const;

		FUNCTION();
		void SetOutline(float value);

		float SpecularPower() const;
		void SetSpecularPower(float power);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateCameraPosition(const DirectX::XMFLOAT3& position);
		void UpdateTransforms(DirectX::FXMMATRIX worldViewProjectionMatrix, DirectX::CXMMATRIX worldMatrix);

	private:
		struct VertexCBufferPerFrame
		{
			DirectX::XMFLOAT3 LightPosition { GameEngine::Vector3Helper::Zero };
			float LightRadius { 50.0f };
		};

		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection { GameEngine::MatrixHelper::Identity };
			DirectX::XMFLOAT4X4 World { GameEngine::MatrixHelper::Identity };
		};

		struct PixelCBufferPerFrame
		{
			DirectX::XMFLOAT4 AmbientColor { DirectX::Colors::Black };
			DirectX::XMFLOAT4 LightColor { DirectX::Colors::White };
			DirectX::XMFLOAT3 LightPosition { 0.0f, 0.0f, 1.0f };
			float padding1;
			DirectX::XMFLOAT3 CameraPosition { GameEngine::Vector3Helper::Zero };
			float Padding2;
		};

		struct PixelCBufferPerObject
		{
			DirectX::XMFLOAT3 SpecularColor { 1.0f, 1.0f, 1.0f };
			float SpecularPower { 128.0f };
			DirectX::XMFLOAT3 CoverColor { Vector3Helper::One };
			float Outline = 0.f;
		};

		virtual void BeginDraw() override;

		void ResetPixelShaderResources();

		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerObject;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerObject;
		VertexCBufferPerFrame mVertexCBufferPerFrameData;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		PixelCBufferPerFrame mPixelCBufferPerFrameData;
		PixelCBufferPerObject mPixelCBufferPerObjectData;
		bool mVertexCBufferPerFrameDataDirty { true };
		bool mPixelCBufferPerFrameDataDirty { true };
		bool mPixelCBufferPerObjectDataDirty { true };
		std::shared_ptr<GameEngine::Texture2D> mColorMap;
		std::shared_ptr<GameEngine::Texture2D> mSpecularMap;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState { GameEngine::SamplerStates::TrilinearClamp };
	};
}
