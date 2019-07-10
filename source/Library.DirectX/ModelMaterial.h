#pragma once

#include <string>
#include <map>
#include <vector>
#include <gsl\gsl>

namespace GameEngine
{
	class Model;
	class OutputStreamHelper;
	class InputStreamHelper;

    enum class TextureType
    {
        Diffuse = 0,
        SpecularMap,
        Ambient,
        Emissive,
        Heightmap,
        NormalMap,
        SpecularPowerMap,
        DisplacementMap,
        LightMap,
        End
    };

	struct ModelMaterialData final
	{
		std::string Name;
		std::map<TextureType, std::vector<std::string>> Textures;
	};

    class ModelMaterial
    {
    public:
		ModelMaterial(Model& model, InputStreamHelper& streamHelper);
		ModelMaterial(Model& model, ModelMaterialData&& modelMaterialData);
		ModelMaterial(const ModelMaterial&) = default;
		ModelMaterial(ModelMaterial&&) = default;
		ModelMaterial& operator=(const ModelMaterial&) = default;
		ModelMaterial& operator=(ModelMaterial&&) = default;
		~ModelMaterial() = default;

        Model& GetModel();
        const std::string& Name() const;
        const std::map<TextureType, std::vector<std::string>>& Textures() const;

		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(InputStreamHelper& streamHelper);

        gsl::not_null<Model*> mModel;
		ModelMaterialData mData;
    };
}