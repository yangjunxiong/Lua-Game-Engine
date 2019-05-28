#pragma once
#include "IJsonParseHelper.h"

namespace GameEngine
{
	class JsonParseHelperTest : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonParseHelperTest, IJsonParseHelper);

	public:
		class SharedData : public BaseSharedData
		{
			RTTI_DECLARATIONS(JsonParseHelperTest::SharedData, BaseSharedData);

		public:
			virtual gsl::owner<BaseSharedData*> Create() const override;
			size_t mMaxDepth = 0;
		};
		size_t mStartHandlerCount = 0;
		size_t mEndHandlerCount = 0;
		size_t mArrayCount = 0;
		bool mIsParsing = false;

		virtual void Initialize() override;
		virtual bool StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		virtual bool EndHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		virtual gsl::owner<IJsonParseHelper*> Create() const override;
	};
}
