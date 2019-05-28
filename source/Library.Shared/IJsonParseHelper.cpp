#include "pch.h"
#include <json/json.h>
#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"

using namespace GameEngine;
using namespace std;

RTTI_DEFINITIONS(IJsonParseHelper);
RTTI_DEFINITIONS(BaseSharedData);

void BaseSharedData::Initialize()
{
	mDepth = 0;
}

JsonParseMaster* BaseSharedData::GetJsonParseMaster() const
{
	return mMaster;
}

void BaseSharedData::IncrementDepth()
{
	++mDepth;
}

void BaseSharedData::DecrementDepth()
{
	if (mDepth > 0)
	{
		--mDepth;
	}
}

size_t BaseSharedData::Depth() const
{
	return mDepth;
}

void BaseSharedData::SetJsonParseMaster(JsonParseMaster* master)
{
	mMaster = master;
}

bool IJsonParseHelper::Equals(const RTTI* other) const
{
	return other != nullptr && TypeIdInstance() == other->TypeIdInstance();
}
