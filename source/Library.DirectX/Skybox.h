#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <gsl\gsl>
#include "ActionRender.h"
#include "MatrixHelper.h"

namespace GameEngine
{
	class SkyboxMaterial;
	class Entity;
	class Camera;

	CLASS(NoLuaAuthority);
	class Skybox final : public ActionRender
	{
		RTTI_DECLARATIONS(Skybox, ActionRender)

	public:
		CONSTRUCTOR();
		Skybox(Entity* entity, const std::string& cubeFile, float scale);
		virtual ~Skybox() = default;

		virtual void Draw() override;

	private:
		std::wstring mCubeMapFileName;
		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
		float mScale;
		std::shared_ptr<SkyboxMaterial> mMaterial;
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount{ 0 };
		bool mUpdateMaterial{ true };
		Camera* mCamera;
	};
}