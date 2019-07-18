#include "pch.h"
#include "StaticMeshRenderComponent.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "RasterizerStates.h"
#include "SamplerStates.h"
#include "Game.h"
#include "Camera.h"
#include "Entity.h"
#include "Utility.h"

using namespace GameEngine;
using namespace std;
using namespace DirectX;
RTTI_DEFINITIONS(StaticMeshRenderComponent);

StaticMeshRenderComponent::StaticMeshRenderComponent(Entity* parent) :
	ActionRender(StaticMeshRenderComponent::TypeIdClass(), parent)
{
	Init();
}

StaticMeshRenderComponent::StaticMeshRenderComponent(RTTI::IdType type, Entity* parent) :
	ActionRender(type, parent)
{
	Init();
}

void StaticMeshRenderComponent::Init()
{
	mGame = Game::GetInstance();
	assert(mGame != nullptr);
	mCamera = static_cast<Camera*>(mGame->Services().GetService(Camera::TypeIdClass()));
	assert(mCamera != nullptr);
	mMaterial.Initialize();

	// Set camera callback to update material buffer
	auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
	if (firstPersonCamera != nullptr)
	{
		Transform* cameraTrans = firstPersonCamera->GetTransform();
		cameraTrans->AddTransformUpdateCallback([this, cameraTrans]()
		{
			auto& pos = cameraTrans->GetWorldPosition().RawVector();
			mMaterial.UpdateCameraPosition(XMFLOAT3(pos.x, pos.y, pos.z));
		});
	}
	auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
	mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
	mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);
	mParent->GetTransform()->AddTransformUpdateCallback(updateMaterialFunc);
}

void StaticMeshRenderComponent::Draw()
{
	const Matrix& worldMatrix = mParent->GetTransform()->GetWorldMatrix();
	if (mUpdateMaterial)
	{
		const XMMATRIX wvp = XMMatrixTranspose((worldMatrix * mCamera->ViewProjectionMatrix()).SIMDMatrix());
		mMaterial.UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix.SIMDMatrix()));
		mUpdateMaterial = false;
	}

	// Draw planet
	mMaterial.DrawIndexed(static_cast<ID3D11Buffer*>(mVertexBuffer.get()), static_cast<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
}

std::shared_ptr<Model> StaticMeshRenderComponent::GetModel() const
{
	return mModel;
}

void StaticMeshRenderComponent::SetModel(std::shared_ptr<Model> model)
{
	mModel = model;
	Mesh* mesh = mModel->Meshes().at(0).get();
	mIndexCount = static_cast<uint32_t>(mesh->Indices().size());
	VertexPositionTextureNormal::CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, static_cast<ID3D11Buffer**>(mVertexBuffer.put()));
	mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), static_cast<ID3D11Buffer**>(mIndexBuffer.put()));
}

void StaticMeshRenderComponent::SetModel(const std::string& fileName)
{
	SetModel(mGame->Content().Load<Model>(Utility::ToWideString(fileName)));
}

StaticMeshMaterial* StaticMeshRenderComponent::GetMaterial()
{
	return &mMaterial;
}
