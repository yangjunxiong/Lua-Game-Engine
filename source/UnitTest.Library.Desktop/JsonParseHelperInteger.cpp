#include "pch.h"
#include "JsonParseHelperInteger.h"
#include "JsonParseHelperObject.h"
#include "Scope.h"
#include <json/json.h>

using namespace GameEngine;
using namespace std;
using IntegerSharedData = JsonParseHelperInteger::SharedData;
using ObjectSharedData = JsonParseHelperObject::SharedData;

RTTI_DEFINITIONS(JsonParseHelperInteger);
RTTI_DEFINITIONS(IntegerSharedData);

bool JsonParseHelperInteger::StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool, size_t)
{
	if (mIsParsing)
	{
		return false;
	}

	IntegerSharedData* intTarget = data.As<IntegerSharedData>();
	if (intTarget != nullptr && value.isInt())
	{
		mIsParsing = true;
		return true;
	}
	
	ObjectSharedData* objectTarget = data.As<ObjectSharedData>();
	if (objectTarget != nullptr && value.isInt())
	{
		Datum* datum = objectTarget->mScope->Find(key);
		if (datum == nullptr || datum->Type() == Datum::DatumType::Unknown || datum->Type() == Datum::DatumType::Integer)
		{
			mIsParsing = true;
			return true;
		}
	}

	return false;
}

bool JsonParseHelperInteger::EndHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool, size_t)
{
	if (!mIsParsing)
	{
		return false;
	}

	IntegerSharedData* target = data.As<IntegerSharedData>();
	if (target != nullptr && value.isInt())
	{
		target->mData = value.asInt();
		mIsParsing = false;
		return true;
	}

	ObjectSharedData* objectTarget = data.As<ObjectSharedData>();
	if (objectTarget != nullptr && value.isInt())
	{
		objectTarget->mScope->Append(key).first.PushBack(value.asInt());
		mIsParsing = false;
		return true;
	}

	return false;
}

gsl::owner<IJsonParseHelper*> JsonParseHelperInteger::Create() const
{
	return new JsonParseHelperInteger();
}

gsl::owner<BaseSharedData*> IntegerSharedData::Create() const
{
	return new IntegerSharedData();
}