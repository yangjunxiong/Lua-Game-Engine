#include "pch.h"
#include "JsonParseHelperTest.h"
#include <json/json.h>

using namespace GameEngine;
using namespace std;
using SharedData = JsonParseHelperTest::SharedData;

RTTI_DEFINITIONS(JsonParseHelperTest);
RTTI_DEFINITIONS(SharedData);

gsl::owner<BaseSharedData*> SharedData::Create() const
{
	return new SharedData();
}

void JsonParseHelperTest::Initialize()
{
	IJsonParseHelper::Initialize();
	mStartHandlerCount = 0;
	mEndHandlerCount = 0;
	mArrayCount = 0;
	mIsParsing = false;
}

bool JsonParseHelperTest::StartHandler(BaseSharedData& data, const std::string&, Json::Value& value, bool mArrayElement, size_t)
{
	SharedData* target = data.As<SharedData>();
	if (target != nullptr)
	{
		++mStartHandlerCount;

		if (value.isObject() && target->Depth() > target->mMaxDepth)
		{
			target->mMaxDepth = target->Depth();
		}

		if (mArrayElement)
		{
			++mArrayCount;
		}
		
		mIsParsing = true;
		return true;
	}
	return false;
}

bool JsonParseHelperTest::EndHandler(BaseSharedData& data, const std::string&, Json::Value& value, bool, size_t)
{
	if (!mIsParsing)
	{
		return false;
	}

	SharedData* target = data.As<SharedData>();
	if (target != nullptr)
	{
		++mEndHandlerCount;

		if (value.isObject() && target->Depth() > target->mMaxDepth)
		{
			target->mMaxDepth = target->Depth();
		}
		
		if (mStartHandlerCount == mEndHandlerCount)
		{
			mIsParsing = false;
		}
		return true;
	}
	return false;
}

gsl::owner<IJsonParseHelper*> JsonParseHelperTest::Create() const
{
	return new JsonParseHelperTest();
}
