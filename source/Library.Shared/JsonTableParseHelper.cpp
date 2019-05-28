#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Scope.h"
#include "Factory.h"
#include "JsonParseMaster.h"
#include <json/json.h>

using namespace GameEngine;
using namespace std;
using SharedData = JsonTableParseHelper::SharedData;

RTTI_DEFINITIONS(JsonTableParseHelper);
RTTI_DEFINITIONS(SharedData);

const HashMap<string, Datum::DatumType> JsonTableParseHelper::sDatumTypeMap =
{
	{ "integer", Datum::DatumType::Integer },
	{ "float", Datum::DatumType::Float },
	{ "vector", Datum::DatumType::Vector },
	{ "matrix", Datum::DatumType::Matrix },
	{ "string", Datum::DatumType::String },
	{ "table", Datum::DatumType::Table }
};

SharedData::SharedData(const shared_ptr<Scope>& scope) :
	mTable(scope)
{}

void SharedData::Initialize()
{
	BaseSharedData::Initialize();
	mStack.Clear();
	mStack.Push(StackFrame(*mTable, false));  // Make sure before each parsing, stack top is always the root scope
}

gsl::owner<BaseSharedData*> SharedData::Create() const
{
	shared_ptr<Scope> cloneScope(mTable->Clone());
	return new SharedData(cloneScope);
}

Scope& SharedData::GetTable()
{
	return *mTable;
}

const Scope& SharedData::GetTable() const
{
	return *mTable;
}

SharedData::StackFrame::StackFrame(Scope& scope, bool inDatum, const std::string& key, size_t valueCount) :
	mScope(&scope),
	mDatumFrame(inDatum),
	mKey(key),
	mValueCount(valueCount)
{}

bool JsonTableParseHelper::StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool /*isArrayElement*/, size_t index)
{
	// Shared data should match
	SharedData* target = data.As<SharedData>();
	if (target == nullptr)
	{
		return false;
	}

	if (target->mStack.IsEmpty() || !target->mStack.Peak().mDatumFrame)  // We are in a table
	{
		return StartTableHandler(*target, key, value);
	}
	else if (key == SharedData::JSON_VALUE_KEY)                           // We are in a datum "value" key
	{
		return StartValueHandler(*target, value, index);
	}
	else if (key == SharedData::JSON_TYPE_KEY)  // Since table handler already parsed the type, just pass it
	{
		return true;
	}

	return false;
}

bool JsonTableParseHelper::EndHandler(BaseSharedData& data, const std::string& key, Json::Value& /*value*/, bool /*isArrayElement*/, size_t /*index*/)
{
	// Shared data should match
	SharedData* target = data.As<SharedData>();
	if (target == nullptr)
	{
		return false;
	}

	SharedData::StackFrame& frame = target->mStack.Peak();
	if (frame.mDatumFrame)
	{
		if (frame.mKey == key && frame.mValueCount == 0)  // All values in the datum are parsed, jump out of current datum
		{
			target->mStack.Pop();
		}
		else if (key == SharedData::JSON_VALUE_KEY)                    // Just finished a value element, mark this element as parsed
		{
			--frame.mValueCount;
		}
	}
	else if (key != SharedData::JSON_CLASS_KEY && key != SharedData::JSON_FILE_KEY)           // Finish all pairs in current table
	{
		target->mStack.Pop();                        // Get out of current nested scope
		--target->mStack.Peak().mValueCount;             // And mark this scope object as parsed in its parent datum frame
	}

	return true;
}

gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
{
	return new JsonTableParseHelper();
}

bool JsonTableParseHelper::StartTableHandler(SharedData& data, const std::string& key, Json::Value& value)
{
	// "class" is special key that can exist as string instead of object in the table
	if (key == SharedData::JSON_CLASS_KEY)
	{
		return true;
	}

	// "file" is special key that can replace the actual content of the table
	// Can't find better way but to create a temporary parse master to parse that json file recursively
	if (key == SharedData::JSON_FILE_KEY)
	{
		auto& frame = data.mStack.Peak();
		assert(!frame.mDatumFrame && frame.mScope != nullptr);
		shared_ptr<Scope> scopeToFill(frame.mScope, [](Scope*) {});  // Create a shared pointer that dosen't delete the raw poniter, because the raw one is owned by its parent
		assert(frame.mScope->GetParent() != nullptr);
		SharedData nestedFileData(scopeToFill);
		JsonParseMaster master(nestedFileData);
		JsonTableParseHelper helper;
		master.AddHelper(helper);
		bool result = master.ParseFromFile(value.asString());
		return result;
	}

	// Check this datum object matches the grammar
	if (!value.isObject() || !value.isMember(SharedData::JSON_TYPE_KEY) || !value.isMember(SharedData::JSON_VALUE_KEY))
	{
		return false;
	}

	// Create datum if one with same key doesn't exist, or use existing one
	Scope& scope = *data.mStack.Peak().mScope;
	Datum* datum = &scope.Append(key).first;

	// Decide type here so that order doesn't matter anymore
	string typeString = value[SharedData::JSON_TYPE_KEY].asString();
	HashMap<std::string, Datum::DatumType>::ConstIterator it;
	if (sDatumTypeMap.ContainsKey(typeString, it))  // Primitive type
	{
		// If this is a prescribed attribute and type doesn't match, will blow up
		datum->SetType(it->second);

		// Push a new datum frame to stack
		Json::Value& actualValue = value[SharedData::JSON_VALUE_KEY];
		SharedData::StackFrame frame(scope, true, key, actualValue.isArray() ? actualValue.size() : 1);
		data.mStack.Push(frame);
		return true;
	}

	// Unsupported type
	return false;
}

bool JsonTableParseHelper::StartValueHandler(SharedData& data, Json::Value& value, size_t index)
{
	SharedData::StackFrame& frame = data.mStack.Peak();
	Datum& datum = (*frame.mScope)[frame.mKey];
	Datum::DatumType type = datum.Type();

	if (type == Datum::DatumType::Table)
	{
		if (value.isObject())
		{
			// Create a nested scope or use existing one
			Scope* childScope = nullptr;
			if (datum.Size() > index)
			{
				childScope = &datum.AsTable(index);
			}
			else if (value.isMember(SharedData::JSON_CLASS_KEY))
			{
				string className = value[SharedData::JSON_CLASS_KEY].asString();
				childScope = Factory<Scope>::Create(className);
				assert(childScope != nullptr);
				frame.mScope->Adopt(*childScope, frame.mKey);
			}
			else
			{
				childScope = &frame.mScope->AppendScope(frame.mKey);
			}

			// Push a nested scope frame to stack
			SharedData::StackFrame nestedFrame(*childScope, false);
			data.mStack.Push(nestedFrame);

			return true;
		}
		return false;
	}
	else
	{
		// SetSize has a little overhead since we are setting the value right afterwards, but with this we don't need to pushback with different types
		// If this is a prescribed external datum, will blow up if we try to grow the size
		if (datum.Size() <= index)
		{
			datum.SetSize(index + 1);
		}

		if (type == Datum::DatumType::Integer)
		{
			if (value.isInt())
			{
				datum.Set(value.asInt(), index);
				return true;
			}
			return false;
		}
		else if (type == Datum::DatumType::Float)
		{
			if (value.isDouble())
			{
				datum.Set(value.asFloat(), index);
				return true;
			}
			return false;
		}
		else
		{
			return datum.SetFromString(value.asString(), index);
		}
	}
}
