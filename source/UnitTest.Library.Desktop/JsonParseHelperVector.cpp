#include "pch.h"
#include "JsonParseHelperVector.h"
#include "JsonTableParseHelper.h"
#include "Scope.h"
#include <json/json.h>

using namespace GameEngine;
using namespace std;
using SharedData = JsonTableParseHelper::SharedData;

RTTI_DEFINITIONS(JsonParseHelperVector);

bool JsonParseHelperVector::StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool /*isArrayElement*/, size_t index)
{
	SharedData* target = data.As<SharedData>();
	if (target == nullptr)
	{
		return false;
	}

	if (key != SharedData::JSON_VALUE_KEY || !value.isString())
	{
		return false;
	}

	SharedData::StackFrame frame = target->mStack.Peak();
	if (!frame.mDatumFrame)
	{
		return false;
	}

	Datum& datum = *frame.mScope->Find(frame.mKey);
	if (datum.Type() != Datum::DatumType::Vector)
	{
		return false;
	}

	float f[4];
	if (sscanf_s(value.asString().c_str(), "[%f,%f,%f,%f]", &f[0], &f[1], &f[2], &f[3]) == 4)
	{
		if (datum.Size() < index)
		{
			datum.PushBack(glm::vec4(f[0], f[1], f[2], f[3]));
		}
		else
		{
			datum.Set(glm::vec4(f[0], f[1], f[2], f[3]), index);
		}
		return true;
	}
	return false;
}

bool JsonParseHelperVector::EndHandler(BaseSharedData& data, const std::string& /*key*/, Json::Value& /*value*/, bool /*isArrayElement*/, size_t /*index*/)
{
	SharedData* target = data.As<SharedData>();
	if (target == nullptr)
	{
		return false;
	}

	SharedData::StackFrame frame = target->mStack.Peak();
	--frame.mValueCount;
	return true;
}

gsl::owner<IJsonParseHelper*> JsonParseHelperVector::Create() const
{
	return new JsonParseHelperVector();
}
