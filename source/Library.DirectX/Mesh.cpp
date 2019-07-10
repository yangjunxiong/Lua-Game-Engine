#include "pch.h"
#include "Mesh.h"
#include "GameException.h"
#include "StreamHelper.h"
#include "ModelMaterial.h"
#include "Model.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace GameEngine
{
	Mesh::Mesh(Model& model, InputStreamHelper& streamHelper) :
		mModel(&model)
	{
		Load(streamHelper);
	}

	Mesh::Mesh(Model& model, MeshData&& meshData) :
		mModel(&model), mData(move(meshData))
	{
	}

	Model& Mesh::GetModel()
	{
		return *mModel;
	}

	shared_ptr<ModelMaterial> Mesh::GetMaterial()
	{
		return mData.Material;
	}

	const string& Mesh::Name() const
	{
		return mData.Name;
	}

	const vector<XMFLOAT3>& Mesh::Vertices() const
	{
		return mData.Vertices;
	}

	const vector<XMFLOAT3>& Mesh::Normals() const
	{
		return mData.Normals;
	}

	const vector<XMFLOAT3>& Mesh::Tangents() const
	{
		return mData.Tangents;
	}

	const vector<XMFLOAT3>& Mesh::BiNormals() const
	{
		return mData.BiNormals;
	}

	const vector<vector<XMFLOAT3>>& Mesh::TextureCoordinates() const
	{
		return mData.TextureCoordinates;
	}

	const vector<vector<XMFLOAT4>>& Mesh::VertexColors() const
	{
		return mData.VertexColors;
	}

	uint32_t Mesh::FaceCount() const
	{
		return mData.FaceCount;
	}

	const vector<uint32_t>& Mesh::Indices() const
	{
		return mData.Indices;
	}

	void Mesh::CreateIndexBuffer(ID3D11Device& device, gsl::not_null<ID3D11Buffer**> indexBuffer)
	{
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = narrow_cast<uint32_t>(sizeof(uint32_t) * mData.Indices.size());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &mData.Indices[0];

		ThrowIfFailed(device.CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}

	void Mesh::Save(OutputStreamHelper& streamHelper) const
	{
		string materialName = (mData.Material != nullptr ? mData.Material->Name() : "");
		streamHelper << materialName;

		// Serialize name
		streamHelper << mData.Name;

		// Serialize vertices
		streamHelper << narrow_cast<uint32_t>(mData.Vertices.size());
		for (const XMFLOAT3& vertex : mData.Vertices)
		{
			streamHelper << vertex.x << vertex.y << vertex.z;
		}

		// Serialize normals
		streamHelper << narrow_cast<uint32_t>(mData.Normals.size());
		for (const XMFLOAT3& normal : mData.Normals)
		{
			streamHelper << normal.x << normal.y << normal.z;
		}

		// Serialize tangents
		streamHelper << narrow_cast<uint32_t>(mData.Tangents.size());
		for (const XMFLOAT3& tangents : mData.Tangents)
		{
			streamHelper << tangents.x << tangents.y << tangents.z;
		}

		// Serialize binormals
		streamHelper << narrow_cast<uint32_t>(mData.BiNormals.size());
		for (const XMFLOAT3& binormal : mData.BiNormals)
		{
			streamHelper << binormal.x << binormal.y << binormal.z;
		}

		// Serialize texture coordinates
		streamHelper << narrow_cast<uint32_t>(mData.TextureCoordinates.size());
		for (const auto& uvList : mData.TextureCoordinates)
		{
			streamHelper << narrow_cast<uint32_t>(uvList.size());
			for (const XMFLOAT3& uv : uvList)
			{
				streamHelper << uv.x << uv.y << uv.z;
			}
		}

		// Serialize vertex colors
		streamHelper << narrow_cast<uint32_t>(mData.VertexColors.size());
		for (const auto& vertexColorList : mData.VertexColors)
		{
			streamHelper << vertexColorList.size();
			for (const XMFLOAT4& vertexColor : vertexColorList)
			{
				streamHelper << vertexColor.x << vertexColor.y << vertexColor.z << vertexColor.w;
			}
		}

		// Serialize indices
		streamHelper << mData.FaceCount;
		streamHelper << narrow_cast<uint32_t>(mData.Indices.size());
		for (const uint32_t& index : mData.Indices)
		{
			streamHelper << index;
		}
	}

	void Mesh::Load(InputStreamHelper& streamHelper)
	{
		// Deserialize material reference
		{
			string materialName;
			streamHelper >> materialName;

			for (auto material : mModel->Materials())
			{
				if (material->Name() == materialName)
				{
					mData.Material = material;
					break;
				}
			}
		}

		// Deserialize name
		streamHelper >> mData.Name;

		// Deserialize vertices
		{
			uint32_t vertexCount;
			streamHelper >> vertexCount;
			mData.Vertices.reserve(vertexCount);
			for (uint32_t i = 0; i < vertexCount; i++)
			{
				XMFLOAT3 vertex;
				streamHelper >> vertex.x >> vertex.y >> vertex.z;
				mData.Vertices.push_back(vertex);
			}
		}

		// Deserialize normals
		{
			uint32_t normalCount;
			streamHelper >> normalCount;
			mData.Normals.reserve(normalCount);
			for (uint32_t i = 0; i < normalCount; i++)
			{
				XMFLOAT3 normal;
				streamHelper >> normal.x >> normal.y >> normal.z;
				mData.Normals.push_back(normal);
			}
		}

		// Deserialize tangents
		{
			uint32_t tangentCount;
			streamHelper >> tangentCount;
			mData.Tangents.reserve(tangentCount);
			for (uint32_t i = 0; i < tangentCount; i++)
			{
				XMFLOAT3 tangent;
				streamHelper >> tangent.x >> tangent.y >> tangent.z;
				mData.Tangents.push_back(tangent);
			}
		}

		// Deserialize binormals
		{
			uint32_t binormalCount;
			streamHelper >> binormalCount;
			mData.BiNormals.reserve(binormalCount);
			for (uint32_t i = 0; i < binormalCount; i++)
			{
				XMFLOAT3 binormal;
				streamHelper >> binormal.x >> binormal.y >> binormal.z;
				mData.BiNormals.push_back(binormal);
			}
		}

		// Deserialize texture coordinates
		{
			uint32_t textureCoordinateCount;
			streamHelper >> textureCoordinateCount;
			mData.TextureCoordinates.reserve(textureCoordinateCount);
			for (uint32_t i = 0; i < textureCoordinateCount; i++)
			{
				uint32_t uvListCount;
				streamHelper >> uvListCount;
				if (uvListCount > 0)
				{
					std::vector<XMFLOAT3> uvs;
					uvs.reserve(uvListCount);
					for (uint32_t j = 0; j < uvListCount; j++)
					{
						XMFLOAT3 uv;
						streamHelper >> uv.x >> uv.y >> uv.z;
						uvs.push_back(uv);
					}

					mData.TextureCoordinates.push_back(move(uvs));
				}
			}
		}

		// Deserialize vertex colors
		{
			uint32_t vertexColorCount;
			streamHelper >> vertexColorCount;
			mData.VertexColors.reserve(vertexColorCount);
			for (uint32_t i = 0; i < vertexColorCount; i++)
			{
				uint32_t vertexColorListCount;
				streamHelper >> vertexColorListCount;
				if (vertexColorListCount > 0)
				{
					std::vector<XMFLOAT4> vertexColors;
					vertexColors.reserve(vertexColorListCount);
					for (uint32_t j = 0; j < vertexColorListCount; j++)
					{
						XMFLOAT4 vertexColor;
						streamHelper >> vertexColor.x >> vertexColor.y >> vertexColor.z >> vertexColor.w;
						vertexColors.push_back(vertexColor);
					}

					mData.VertexColors.push_back(move(vertexColors));
				}
			}
		}

		// Deserialize indexes	
		{
			streamHelper >> mData.FaceCount;
			uint32_t indexCount;
			streamHelper >> indexCount;
			mData.Indices.reserve(indexCount);
			for (uint32_t i = 0; i < indexCount; i++)
			{
				uint32_t index;
				streamHelper >> index;
				mData.Indices.push_back(index);
			}
		}
	}
}