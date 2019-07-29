#include "pch.h"
#include "Skybox.h"
#include "Game.h"
#include "GameException.h"
#include "FirstPersonCamera.h"
#include "Model.h"
#include "Mesh.h"
#include "SkyboxMaterial.h"
#include "VertexDeclarations.h"
#include "VectorHelper.h"
#include "TextureCube.h"
#include "Camera.h"
#include "Entity.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace GameEngine;

RTTI_DEFINITIONS(Skybox)

Skybox::Skybox(Entity* entity, const std::string& cubeFile, float scale) :
	ActionRender(entity),
	mCubeMapFileName(std::wstring(cubeFile.begin(), cubeFile.end())),
	mScale(scale)
{
	Game* game = Game::GetInstance();
	const auto model = game->Content().Load<Model>(L"Models\\Sphere.obj.bin");
	Mesh* mesh = model->Meshes().at(0).get();
	VertexPosition::CreateVertexBuffer(game->Direct3DDevice(), *mesh, not_null<ID3D11Buffer * *>(mVertexBuffer.put()));
	mesh->CreateIndexBuffer(*game->Direct3DDevice(), not_null<ID3D11Buffer * *>(mIndexBuffer.put()));
	mIndexCount = narrow<uint32_t>(mesh->Indices().size());

	auto textureCube = game->Content().Load<TextureCube>(mCubeMapFileName);
	mMaterial = make_shared<SkyboxMaterial>(*game, textureCube);
	mMaterial->Initialize();

	mCamera = game->GetCamera();
	auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
	mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
	mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);
	auto* transform = mCamera->GetTransform();
	mCamera->GetTransform()->AddTransformUpdateCallback([this, transform]()
		{
			auto& vector = transform->GetWorldPosition().RawVector();
			const XMFLOAT3 currentPosition(vector.x, vector.y, vector.z);
			XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * XMMatrixTranslation(currentPosition.x, currentPosition.y, currentPosition.z));
		});
}
	
void Skybox::Draw()
{
	if (mUpdateMaterial)
	{
		const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * XMLoadFloat4x4(&mCamera->ViewProjectionMatrix().RawMatrix()));
		mMaterial->UpdateTransforms(wvp);
		mUpdateMaterial = false;
	}

	mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
}
