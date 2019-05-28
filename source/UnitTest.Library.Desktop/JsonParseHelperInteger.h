#pragma once
#include "IJsonParseHelper.h"

namespace GameEngine
{
	class JsonParseHelperInteger : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonParseHelperInteger, IJsonParseHelper);

	public:
		class SharedData : public BaseSharedData
		{
			RTTI_DECLARATIONS(JsonParseHelperInteger::SharedData, BaseSharedData);

		public:
			virtual gsl::owner<BaseSharedData*> Create() const override;
			int mData = 0;
		};
		bool mIsParsing = false;

		virtual void Initialize() override { mIsParsing = false; }
		virtual bool StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		virtual bool EndHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		virtual gsl::owner<IJsonParseHelper*> Create() const override;
	};
}