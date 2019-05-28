#include "pch.h"
#include <json/json.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

using namespace GameEngine;
using namespace std;

JsonParseMaster::JsonParseMaster(BaseSharedData& data) :
	mData(&data)
{
	mData->SetJsonParseMaster(this);
}

JsonParseMaster::JsonParseMaster(JsonParseMaster&& other) :
	mIsClone(other.mIsClone),
	mDataIsClone(other.mDataIsClone),
	mFileName(move(other.mFileName)),
	mData(other.mData),
	mHelpers(move(other.mHelpers))
{
	mData->SetJsonParseMaster(this);
	other.mIsClone = false;
	other.mDataIsClone = false;
	other.mData = nullptr;
}

JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& other)
{
	if (this != &other)
	{
		Clear();

		mIsClone = other.mIsClone;
		mDataIsClone = other.mDataIsClone;
		mFileName = move(other.mFileName);
		mData = other.mData;
		mHelpers = move(other.mHelpers);
		mData->SetJsonParseMaster(this);

		other.mIsClone = false;
		other.mDataIsClone = false;
		other.mData = nullptr;
	}
	return *this;
}

JsonParseMaster::~JsonParseMaster()
{
	Clear();
}

gsl::owner<JsonParseMaster*> JsonParseMaster::Clone() const
{
	BaseSharedData* cloneData = mData->Create();
	JsonParseMaster* clone = new JsonParseMaster(*cloneData);
	clone->mIsClone = true;
	clone->mDataIsClone = true;

	clone->mHelpers.Reserve(mHelpers.Size());
	for (const auto& helper : mHelpers)
	{
		clone->mHelpers.PushBack(helper->Create());
	}
	return clone;
}

bool JsonParseMaster::AddHelper(IJsonParseHelper& helper)
{
	if (mIsClone)
	{
		throw exception("Can't add helper to a clone");
	}

	// Duplicate helper check
	for (const auto& item : mHelpers)
	{
		if (item->Equals(&helper))
		{
			return false;
		}
	}

	// Append to last
	mHelpers.PushBack(&helper);
	return true;
}

bool JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
{
	if (mIsClone)
	{
		throw exception("Can't remove helper from a clone");
	}

	for (auto it = mHelpers.begin(); it != mHelpers.end(); ++it)
	{
		auto& item = *it;
		if (item->Equals(&helper))
		{
			mHelpers.Remove(it);
			return true;
		}
	}

	return false;
}

bool JsonParseMaster::Parse(const std::string& jsonString)
{
	stringstream stream(jsonString);
	return Parse(stream);
}

bool JsonParseMaster::Parse(std::istream& stream)
{
	// Parse stream to root json object, might throw exception on parse error
	Json::Value root;
	stream >> root;

	// Initialize helpers
	for (auto& helper : mHelpers)
	{
		helper->Initialize();
	}

	// Initialize data
	mData->Initialize();

	return ParseObject(root);
}

bool JsonParseMaster::ParseFromFile(const std::string& path)
{
	mFileName = path;
	ifstream stream(path);
	if (stream.fail())
	{
		throw std::exception("File doesn't exist");
	}

	return Parse(stream);
}

const std::string& JsonParseMaster::GetFileName() const
{
	return mFileName;
}

BaseSharedData* JsonParseMaster::GetSharedData()
{
	return mData;
}

const BaseSharedData* JsonParseMaster::GetSharedData() const
{
	return mData;
}

bool JsonParseMaster::IsClone() const
{
	return mIsClone;
}

void JsonParseMaster::SetSharedData(BaseSharedData& data)
{
	if (mIsClone && mDataIsClone)
	{
		delete mData;
		mDataIsClone = false;
	}
	else
	{
		mData->SetJsonParseMaster(nullptr);
	}
	
	mData = &data;
	mData->SetJsonParseMaster(this);
}

void JsonParseMaster::Clear()
{
	if (mIsClone)
	{
		for (const auto& helper : mHelpers)
		{
			delete helper;
		}
		if (mDataIsClone)
		{
			delete mData;
		}
	}
}

bool JsonParseMaster::ParsePair(const std::string& key, Json::Value& value, bool isArrayElement, size_t index)
{
	bool success = false;

	switch (value.type())
	{
	case Json::ValueType::objectValue:
		for (auto& helper : mHelpers)
		{
			if (helper->StartHandler(*mData, key, value, isArrayElement, index))
			{
				// Parse all child nodes
				success = ParseObject(value);

				// All nodes are parsed, try to end this node
				if (success)
				{
					success = helper->EndHandler(*mData, key, value, isArrayElement, index);
					if (success)  // End successfully, stop parsing, otherwise go to next helper
					{
						break;
					}
				}
				// Can't parse some nodes, end current helper and terminate parsing
				else
				{
					helper->EndHandler(*mData, key, value, isArrayElement, index);
					break;
				}
			}
		}
		break;
	case Json::ValueType::arrayValue:
		success = true;
		for (Json::ArrayIndex i = 0; i < value.size(); ++i)
		{
			if (!ParsePair(key, value[i], true, i))
			{
				success = false;
				break;
			}
		}
		break;
	default:
		for (auto& helper : mHelpers)
		{
			if (helper->StartHandler(*mData, key, value, isArrayElement, index))
			{
				success = helper->EndHandler(*mData, key, value, isArrayElement, index);
				if (success)
				{
					break;
				}
			}
		}
	}

	return success;
}

bool JsonParseMaster::ParseObject(Json::Value& value)
{
	if (value.isObject())
	{
		bool success = true;

		mData->IncrementDepth();
		auto keys = value.getMemberNames();
		for (const auto& childKey : keys)
		{
			if (!ParsePair(childKey, value[childKey], false, 0))
			{
				success = false;
				break;
			}
		}
		mData->DecrementDepth();

		return success;
	}

	return false;
}
