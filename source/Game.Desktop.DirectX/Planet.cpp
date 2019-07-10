#include "pch.h"
#include "Planet.h"
#include "Game.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "RenderingGame.h"
#include "RasterizerStates.h"
#include <random>
#include <DirectXTK/PrimitiveBatch.h>

using namespace std;
using namespace gsl;
using namespace GameEngine;
using namespace DirectX;
RTTI_DEFINITIONS(Planet);

Planet::Planet(GameEngine::Game& game, const std::shared_ptr<GameEngine::Camera>& camera, const std::wstring& modelFile, const std::wstring& textureFile, const std::wstring& specularFile) :
	DrawableGameComponent(game, camera),
	mModelFile(modelFile),
	mTextureFile(textureFile),
	mSpecularFile(specularFile),
	mMaterial(game)
{
	mRenderGame = mGame->As<Rendering::RenderingGame>();
}

PlanetMaterial* Planet::GetMaterial()
{
	return &mMaterial;
}

void Planet::Initialize()
{
	InitPlanet();
}

void Planet::InitPlanet()
{
	// Load model, texture and specular map
	mModel = mGame->Content().Load<Model>(mModelFile);
	Mesh* mesh = mModel->Meshes().at(0).get();
	mIndexCount = narrow_cast<uint32_t>(mesh->Indices().size());
	VertexPositionTextureNormal::CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, not_null<ID3D11Buffer * *>(mVertexBuffer.put()));
	mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), not_null<ID3D11Buffer * *>(mIndexBuffer.put()));

	// Set material
	mMaterial.SetSamplerState(SamplerStates::TrilinearWrap);
	mMaterial.SetColorMap(mGame->Content().Load<Texture2D>(mTextureFile));
	mMaterial.SetLightPosition(Vector3Helper::Zero);
	mMaterial.SetLightRadius(1000000.f);  // Sun light is infinite distance
	if (!mSpecularFile.empty())
	{
		mMaterial.SetSpecularMap(mGame->Content().Load<Texture2D>(mSpecularFile));
	}
	mMaterial.Initialize();

	// Set camera callback to update material buffer
	auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
	if (firstPersonCamera != nullptr)
	{
		firstPersonCamera->AddPositionUpdatedCallback([this]()
			{
				mMaterial.UpdateCameraPosition(mCamera->Position());
			});
	}
	auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
	mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
	mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

	// Random a position
	mRotationAngle = static_cast<float>(rand());
}

void Planet::Update(const GameTime& gameTime)
{
	float deltaTime = gameTime.ElapsedGameTimeSeconds().count();
	float changeStep = deltaTime;
	if (mSpinSpeed != 0.f)
	{
		mSpinAngle += mSpinSpeed * changeStep;
		auto rot = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&mSpinNormal));
		auto finalRot = XMQuaternionMultiply(rot, XMQuaternionRotationRollPitchYaw(0.f, mSpinAngle, 0.f));
		SetLocalRotation(finalRot);
		mUpdateMaterial = true;
	}

	if (mRotationSpeed != 0.f && mParent != nullptr)
	{
		mRotationAngle += mRotationSpeed * changeStep;
		auto rotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&mOrbitNormal), mRotationAngle);
		XMFLOAT3 forward;
		MatrixHelper::GetForward(rotationMatrix, forward);
		auto extendVector = XMVectorScale(XMLoadFloat3(&forward), mOrbitRadius);
		auto targetPosition = XMVectorAdd(XMLoadFloat4(&mParent->GetWorldPosition()), extendVector);
		SetWorldPosition(targetPosition);
		mUpdateMaterial = true;
	}
}

void Planet::Draw(const GameTime& /*gameTime*/)
{
	if (mUpdateMaterial)
	{
		const XMFLOAT4X4 world = GetWorldMatrix();
		const XMMATRIX worldMatrix = XMLoadFloat4x4(&world);
		const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
		mMaterial.UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
		mUpdateMaterial = false;
	}

	// Draw planet
	mMaterial.DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
}