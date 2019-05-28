#include "pch.h"
#include "JsonParseHelperObject.h"
#include "Scope.h"
#include <json/json.h>

using namespace GameEngine;
using namespace std;
using SharedData = JsonParseHelperObject::SharedData;

RTTI_DEFINITIONS(JsonParseHelperObject);
RTTI_DEFINITIONS(SharedData);

SharedData::SharedData()
{
	mScope = new Scope();
}

SharedData::~SharedData()
{
	delete mScope;
}

gsl::owner<BaseSharedData*> SharedData::Create() const
{
	return new SharedData();
}

void JsonParseHelperObject::Initialize()
{
	IJsonParseHelper::Initialize();
	mStack.Clear();
}

bool JsonParseHelperObject::StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool, size_t)
{
	SharedData* target = data.As<SharedData>();
	if (target != nullptr && value.isObject())    // Check input type is valid
	{
		Datum* datum = target->mScope->Find(key);
		if (datum == nullptr || datum->Type() == Datum::DatumType::Unknown || datum->Type() == Datum::DatumType::Table)  // Check we can append new scope to the datum
		{
			mStack.PushBack(target->mScope);
			target->mScope = &target->mScope->AppendScope(key);
			return true;
		}
	}

	return false;
}

bool JsonParseHelperObject::EndHandler(BaseSharedData& data, const std::string&, Json::Value& value, bool, size_t)
{
	SharedData* target = data.As<SharedData>();
	if (target != nullptr && value.isObject())
	{
		target->mScope = mStack.Back();
		mStack.PopBack();
		return true;
	}

	return false;
}

gsl::owner<IJsonParseHelper*> JsonParseHelperObject::Create() const
{
	return new JsonParseHelperObject();
}