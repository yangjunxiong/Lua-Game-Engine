#pragma once

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "RTTI.h"

namespace GameEngine
{
    class Game;
    class Mesh;
    class ModelMaterial;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct ModelData final
	{
		std::vector<std::shared_ptr<Mesh>> Meshes;
		std::vector<std::shared_ptr<ModelMaterial>> Materials;
	};

    class Model final : public RTTI
    {
		RTTI_DECLARATIONS(Model, RTTI)

    public:
		Model() = default;
		Model(const std::string& filename);
		Model(std::ifstream& file);
		Model(ModelData&& modelData);
		Model(const Model&) = default;
		Model(Model&&) = default;
		Model& operator=(const Model&) = default;
		Model& operator=(Model&&) = default;
		~Model() = default;

        bool HasMeshes() const;
        bool HasMaterials() const;

        const std::vector<std::shared_ptr<Mesh>>& Meshes() const;
		const std::vector<std::shared_ptr<ModelMaterial>>& Materials() const;

		ModelData& Data();

		void Save(const std::string& filename) const;
		void Save(std::ofstream& file) const;

    private:
		void Load(const std::string& filename);
		void Load(std::ifstream& file);

		ModelData mData;
    };
}