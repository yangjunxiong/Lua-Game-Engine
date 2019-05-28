#pragma once
#include "IJsonParseHelper.h"

namespace GameEngine
{
	class JsonParseHelperVector : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonParseHelperVector, IJsonParseHelper);

	public:
		virtual bool StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		virtual bool EndHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		virtual gsl::owner<IJsonParseHelper*> Create() const override;
	};
}
