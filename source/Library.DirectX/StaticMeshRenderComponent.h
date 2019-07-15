#pragma once
#include "ActionRender.h"
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <gsl/gsl>
#include <gsl/gsl_util>
#include "StaticMeshMaterial.h"

namespace GameEngine
{
	class Model;
	class Game;
	class Camera;
	class StaticMeshMaterial;

	CLASS();
	class StaticMeshRenderComponent : public ActionRender
	{
		RTTI_DECLARATIONS(StaticMeshRenderComponent, ActionRender);

	public:
		CONSTRUCTOR();
		StaticMeshRenderComponent(Entity* parent);
		virtual void Draw() override;

		std::shared_ptr<Model> GetModel() const;
		void SetModel(std::shared_ptr<Model> model);
		FUNCTION();
		void SetModel(const std::string& fileName);

		FUNCTION();
		StaticMeshMaterial* GetMaterial();

	protected:
		StaticMeshRenderComponent(RTTI::IdType type, Entity* parent);
		inline void Init();

		Game* mGame = nullptr;
		Camera* mCamera;
		StaticMeshMaterial mMaterial;
		std::shared_ptr<Model> mModel;
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount = 0;
		bool mUpdateMaterial = true;
	};
}
