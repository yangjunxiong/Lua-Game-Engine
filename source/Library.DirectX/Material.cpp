#include "Material.h"
#include "pch.h"
#include "Material.h"
#include "Game.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace std;
using namespace std::placeholders;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace GameEngine
{
	RTTI_DEFINITIONS(Material)

	const map<ShaderStages, RTTI::IdType> Material::ShaderStageTypeMap
	{
		{ ShaderStages::VS, VertexShader::TypeIdClass() },
		{ ShaderStages::PS, PixelShader::TypeIdClass() },
	};

	const map<RTTI::IdType, ShaderStages> Material::TypeShaderStageMap
	{
		{ VertexShader::TypeIdClass(), ShaderStages::VS },
		{ PixelShader::TypeIdClass(), ShaderStages::PS },
	};

	const map<ShaderStages, Material::ShaderStageCallInfo> Material::ShaderStageCalls
	{		
		{
			ShaderStages::VS,
			{
				SetVSShader,
				bind(&ID3D11DeviceContext::VSSetConstantBuffers, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::VSSetShaderResources, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::VSSetSamplers, _1, _2, _3, _4),
			}
		},
		{
			ShaderStages::PS,
			{
				SetPSShader,
				bind(&ID3D11DeviceContext::PSSetConstantBuffers, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::PSSetShaderResources, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::PSSetSamplers, _1, _2, _3, _4),
			}
		},
	};

	std::vector<ID3D11ShaderResourceView*> Material::EmptySRVs;

	Material::Material(Game& game, D3D_PRIMITIVE_TOPOLOGY topology) :
		mGame(&game),
		mTopology(topology)
	{
	}

	shared_ptr<Shader> Material::GetShader(ShaderStages shaderStage)
	{
		assert(ShaderStageIsProgrammable(shaderStage));

		auto shaderStageInfo = GetDataForShaderStage(shaderStage);
		return (shaderStageInfo.first ? shaderStageInfo.second->Shader : nullptr);
	}

	ID3D11ClassInstance* Material::GetShaderClassInstance(ShaderStages shaderStage)
	{
		assert(ShaderStageIsProgrammable(shaderStage));

		auto shaderStageInfo = GetDataForShaderStage(shaderStage);
		return (shaderStageInfo.first ? shaderStageInfo.second->ShaderClassInstance : nullptr);
	}

	void Material::RemoveConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		auto shaderStageInfo = GetDataForShaderStage(shaderStage);
		if (shaderStageInfo.first)
		{;
			auto& constantBuffers = shaderStageInfo.second->ConstantBuffers;
			constantBuffers.erase(find(constantBuffers.begin(), constantBuffers.end(), constantBuffer));
		}
	}

	void Material::ClearConstantBuffers(ShaderStages shaderStage)
	{
		auto shaderStageInfo = GetDataForShaderStage(shaderStage);
		if (shaderStageInfo.first)
		{
			shaderStageInfo.second->ConstantBuffers.clear();
		}
	}

	void Material::RemoveShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		auto shaderStageInfo = GetDataForShaderStage(shaderStage);
		if (shaderStageInfo.first)
		{
			auto& shaderResources = shaderStageInfo.second->ShaderResources;
			shaderResources.erase(find(shaderResources.begin(), shaderResources.end(), shaderResource));
		}
	}

	void Material::ClearShaderResources(ShaderStages shaderStage)
	{
		auto shaderStageInfo = GetDataForShaderStage(shaderStage);
		if (shaderStageInfo.first)
		{
			shaderStageInfo.second->ShaderResources.clear();
		}
	}

	void Material::RemoveSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		auto shaderStageInfo = GetDataForShaderStage(shaderStage);
		if (shaderStageInfo.first)
		{
			auto& samplerStates = shaderStageInfo.second->SamplerStates;
			samplerStates.erase(find(samplerStates.begin(), samplerStates.end(), samplerState));
		}
	}

	void Material::ClearSamplerStates(ShaderStages shaderStage)
	{
		auto shaderStageInfo = GetDataForShaderStage(shaderStage);
		if (shaderStageInfo.first)
		{
			shaderStageInfo.second->SamplerStates.clear();
		}
	}

	void Material::Initialize()
	{	
	}

	void Material::Draw()
	{
		BeginDraw();

		if (mDrawCallback != nullptr)
		{
			mDrawCallback();
		}

		EndDraw();
	}

	void Material::Draw(not_null<ID3D11Buffer*> vertexBuffer, uint32_t vertexCount, uint32_t startVertexLocation, uint32_t offset)
	{
		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		BeginDraw();

		const uint32_t stride = VertexSize();
		ID3D11Buffer* const vertexBuffers[]{ vertexBuffer };
		direct3DDeviceContext->IASetVertexBuffers(0, narrow_cast<uint32_t>(size(vertexBuffers)), vertexBuffers, &stride, &offset);

		direct3DDeviceContext->Draw(vertexCount, startVertexLocation);

		EndDraw();
	}

	void Material::DrawIndexed(not_null<ID3D11Buffer*> vertexBuffer, not_null<ID3D11Buffer*> indexBuffer, uint32_t indexCount, DXGI_FORMAT format, uint32_t startIndexLocation, uint32_t baseVertexLocation, uint32_t vertexOffset, uint32_t indexOffset)
	{
		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();

		BeginDraw();

		const uint32_t stride = VertexSize();
		const uint32_t offset = 0;
		ID3D11Buffer* const vertexBuffers[]{ vertexBuffer };
		direct3DDeviceContext->IASetVertexBuffers(0, narrow_cast<uint32_t>(size(vertexBuffers)), vertexBuffers, &stride, &vertexOffset);
		direct3DDeviceContext->IASetIndexBuffer(indexBuffer, format, indexOffset);

		direct3DDeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

		EndDraw();
	}

	void Material::BeginDraw()
	{
		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(mTopology);

		assert(mInputLayout != nullptr);
		direct3DDeviceContext->IASetInputLayout(mInputLayout.get());

		for (const auto& entry : mShaderStageData)
		{
			const ShaderStageData& shaderStageData = entry.second;
			auto& shaderStageCallInfo = ShaderStageCalls.at(entry.first);

			shaderStageCallInfo.SetShader(*direct3DDeviceContext, shaderStageData);

			if (shaderStageData.ConstantBuffers.size() > 0)
			{
				shaderStageCallInfo.SetConstantBuffers(direct3DDeviceContext.get(), 0, narrow_cast<uint32_t>(shaderStageData.ConstantBuffers.size()), shaderStageData.ConstantBuffers.data());
			}

			if (shaderStageData.ShaderResources.size() > 0)
			{
				shaderStageCallInfo.SetShaderResources(direct3DDeviceContext.get(), 0, narrow_cast<uint32_t>(shaderStageData.ShaderResources.size()), shaderStageData.ShaderResources.data());
			}

			if (shaderStageData.SamplerStates.size() > 0)
			{
				shaderStageCallInfo.SetSamplers(direct3DDeviceContext.get(), 0, narrow_cast<uint32_t>(shaderStageData.SamplerStates.size()), shaderStageData.SamplerStates.data());
			}
		}

		if (mUpdateMaterialCallback != nullptr)
		{
			mUpdateMaterialCallback();
		}
	}

	void Material::EndDraw()
	{
		if (mAutoUnbindShaderResourcesEnabled)
		{
			for (const auto& entry : mShaderStageData)
			{
				const ShaderStageData& shaderStageData = entry.second;
				auto& shaderStageCallInfo = ShaderStageCalls.at(entry.first);

				if (shaderStageData.ShaderResources.size() > 0)
				{
					assert(EmptySRVs.size() >= shaderStageData.ShaderResources.size());
					shaderStageCallInfo.SetShaderResources(mGame->Direct3DDeviceContext().get(), 0, narrow_cast<uint32_t>(shaderStageData.ShaderResources.size()), EmptySRVs.data());
				}
			}
		}
	}

	pair<bool, Material::ShaderStageData*> Material::GetDataForShaderStage(ShaderStages shaderStage)
	{
		auto it = mShaderStageData.find(shaderStage);
		bool found = (it != mShaderStageData.end());
		ShaderStageData* shaderStageData = (found ? shaderStageData = &(it->second) : nullptr);

		return make_pair(found, shaderStageData);
	}

	void Material::SetVSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData)
	{
		assert(shaderStageData.Shader != nullptr);
		auto vertexShader = static_pointer_cast<VertexShader>(shaderStageData.Shader);
		direct3DDeviceContext.VSSetShader(vertexShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
	}

	void Material::SetPSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			auto pixelShader = static_pointer_cast<PixelShader>(shaderStageData.Shader);
			direct3DDeviceContext.PSSetShader(pixelShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			direct3DDeviceContext.PSSetShader(nullptr, nullptr, 0);
		}
	}
}