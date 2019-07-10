#pragma once
#include "DrawableGameComponent.h"
#include "PlanetMaterial.h"
#include "BasicMaterial.h"
#include "VertexDeclarations.h"

namespace GameEngine
{
	class Model;
}

namespace Rendering
{
	class RenderingGame;
}

CLASS();
class Planet : public DrawableGameComponent
{
	RTTI_DECLARATIONS(Planet, DrawableGameComponent);

public:
	Planet(GameEngine::Game& game, const std::shared_ptr<GameEngine::Camera>& camera,
		const std::wstring& modelFile, const std::wstring& textureFile, const std::wstring& specularFile = std::wstring());
	Planet(const Planet&) = default;
	Planet(Planet&&) = default;
	Planet& operator=(const Planet&) = default;
	Planet& operator=(Planet&&) = default;
	~Planet() = default;

	PlanetMaterial* GetMaterial();

	virtual void Initialize() override;
	virtual void Update(const GameEngine::GameTime& gameTime) override;
	virtual void Draw(const GameEngine::GameTime& gameTime) override;

	PROPERTY();
	float mSpinSpeed = 0.f;

	PROPERTY();
	float mRotationSpeed = 0.f;

	PROPERTY();
	float mOrbitRadius = 0.f;

	DirectX::XMFLOAT3 mOrbitNormal = GameEngine::Vector3Helper::Up;
	DirectX::XMFLOAT3 mSpinNormal = GameEngine::Vector3Helper::Up;

protected:
	inline void InitPlanet();

	Rendering::RenderingGame* mRenderGame;
	std::wstring mModelFile;
	std::wstring mTextureFile;
	std::wstring mSpecularFile;
	PlanetMaterial mMaterial;
	std::shared_ptr<GameEngine::Model> mModel;
	winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
	winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
	std::uint32_t mIndexCount = 0;
	bool mUpdateMaterial = false;

	const static inline int mOrbitPrecision = 24;
	GameEngine::VertexPositionColor mOrbitVertexArray[mOrbitPrecision + 1];

	float mSpinAngle = 0.f;
	float mRotationAngle = 0.f;
};
