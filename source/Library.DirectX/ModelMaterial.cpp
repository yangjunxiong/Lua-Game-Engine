#include "pch.h"
#include "ModelMaterial.h"
#include "StreamHelper.h"

using namespace std;
using namespace gsl;
using namespace GameEngine;

ModelMaterial::ModelMaterial(Model& model, InputStreamHelper& streamHelper) :
	mModel(&model)
{
	Load(streamHelper);
}

ModelMaterial::ModelMaterial(Model& model, ModelMaterialData&& modelMaterialData) :
	mModel(&model), mData(move(modelMaterialData))
{
}

Model& ModelMaterial::GetModel()
{
    return *mModel;
}

const string& ModelMaterial::Name() const
{
    return mData.Name;
}

const map<TextureType, vector<string>>& ModelMaterial::Textures() const
{
	return mData.Textures;
}

void ModelMaterial::Save(OutputStreamHelper& streamHelper) const
{
	streamHelper << mData.Name;

	streamHelper << narrow_cast<uint32_t>(mData.Textures.size());
	for (const auto& texturePair : mData.Textures)
	{
		streamHelper << narrow_cast<int32_t>(texturePair.first);
		streamHelper << narrow_cast<uint32_t>(texturePair.second.size());
		for (const auto& texture : texturePair.second)
		{
			streamHelper << texture;
		}
	}
}

void ModelMaterial::Load(InputStreamHelper& streamHelper)
{
	streamHelper >> mData.Name;

	uint32_t texturesCount;
	streamHelper >> texturesCount;
	for (uint32_t i = 0; i < texturesCount; i++)
	{
		int32_t textureType;
		streamHelper >> textureType;

		vector<string> textures;
		uint32_t textureListCount;
		streamHelper >> textureListCount;
		textures.reserve(textureListCount);
		for (uint32_t j = 0; j < textureListCount; j++)
		{
			string texture;
			streamHelper >> texture;
			textures.push_back(move(texture));
		}	

		mData.Textures.emplace(TextureType(textureType), move(textures));
	}
}