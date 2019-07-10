#pragma once

#include "Material.h"

namespace GameEngine
{
	class BasicMaterial : public Material
	{
		RTTI_DECLARATIONS(BasicMaterial, Material)

	public:
		explicit BasicMaterial(Game& game);
		BasicMaterial(const BasicMaterial&) = default;
		BasicMaterial& operator=(const BasicMaterial&) = default;
		BasicMaterial(BasicMaterial&&) = default;
		BasicMaterial& operator=(BasicMaterial&&) = default;
		virtual ~BasicMaterial() = default;

		void SetSurfaceColor(const DirectX::XMFLOAT4& color);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateTransform(DirectX::CXMMATRIX worldViewProjectionMatrix);

	private:
		void SetSurfaceColor(const float* color);

		winrt::com_ptr<ID3D11Buffer> mVSConstantBuffer;
		winrt::com_ptr<ID3D11Buffer> mPSConstantBuffer;
	};
}
