#pragma once

#include <gsl\gsl>
#include <d3d11.h>

namespace GameEngine
{
	class Model;
    class Material;
    class ModelMaterial;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct MeshData final
	{
		std::shared_ptr<ModelMaterial> Material;
		std::string Name;
		std::vector<DirectX::XMFLOAT3> Vertices;
		std::vector<DirectX::XMFLOAT3> Normals;
		std::vector<DirectX::XMFLOAT3> Tangents;
		std::vector<DirectX::XMFLOAT3> BiNormals;
		std::vector<std::vector<DirectX::XMFLOAT3>> TextureCoordinates;
		std::vector<std::vector<DirectX::XMFLOAT4>> VertexColors;
		std::uint32_t FaceCount{ 0 };
		std::vector<std::uint32_t> Indices;
	};

    class Mesh final
    {
    public:
		Mesh(GameEngine::Model& model, InputStreamHelper& streamHelper);
		Mesh(GameEngine::Model& model, MeshData&& meshData);
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = default;
		Mesh& operator=(const Mesh&) = default;
		Mesh& operator=(Mesh&&) = default;
		~Mesh() = default;

		GameEngine::Model& GetModel();
        std::shared_ptr<ModelMaterial> GetMaterial();
        const std::string& Name() const;

		const std::vector<DirectX::XMFLOAT3>& Vertices() const;
		const std::vector<DirectX::XMFLOAT3>& Normals() const;
		const std::vector<DirectX::XMFLOAT3>& Tangents() const;
		const std::vector<DirectX::XMFLOAT3>& BiNormals() const;
		const std::vector<std::vector<DirectX::XMFLOAT3>>& TextureCoordinates() const;
		const std::vector<std::vector<DirectX::XMFLOAT4>>& VertexColors() const;
		std::uint32_t FaceCount() const;
		const std::vector<std::uint32_t>& Indices() const;

        void CreateIndexBuffer(ID3D11Device& device, gsl::not_null<ID3D11Buffer**> indexBuffer);
		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(InputStreamHelper& streamHelper);

        gsl::not_null<GameEngine::Model*> mModel;
		MeshData mData;
    };
}