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

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace GameEngine
{
	RTTI_DEFINITIONS(Skybox)

	Skybox::Skybox(Game& game, const shared_ptr<Camera>& camera, const wstring& cubeMapFileName, float scale) :
		DrawableGameComponent(game, camera),
		mCubeMapFileName(cubeMapFileName), mScale(scale)
	{
	}

	void Skybox::Initialize()
	{
		const auto model = mGame->Content().Load<Model>(L"Models\\Sphere.obj.bin");
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPosition::CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), not_null<ID3D11Buffer**>(mIndexBuffer.put()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		auto textureCube = mGame->Content().Load<TextureCube>(mCubeMapFileName);
		mMaterial = make_shared<SkyboxMaterial>(*mGame, textureCube);
		mMaterial->Initialize();

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->AddPositionUpdatedCallback([this]() {
				const XMFLOAT3& currentPosition = mCamera->Position();
				XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * XMMatrixTranslation(currentPosition.x, currentPosition.y, currentPosition.z));
			});
		}
	}
	
	void Skybox::Draw(const GameTime&)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp);
			mUpdateMaterial = false;
		}

		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
	}
}