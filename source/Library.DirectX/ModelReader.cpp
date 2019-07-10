#include "pch.h"
#include "ModelReader.h"
#include "Utility.h"

using namespace std;

namespace GameEngine
{
	RTTI_DEFINITIONS(ModelReader)

	ModelReader::ModelReader(Game& game) :
		ContentTypeReader(game, Model::TypeIdClass())
	{
	}

	shared_ptr<Model> ModelReader::_Read(const wstring& assetName)
	{
		return make_shared<Model>(Utility::ToString(assetName));
	}
}