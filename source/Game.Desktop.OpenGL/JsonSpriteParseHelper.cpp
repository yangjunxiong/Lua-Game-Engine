#include "pch.h"
#include "JsonSpriteParseHelper.h"
#include <json/json.h>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace GameEngine;
using namespace std;

Vector <SpriteInfo> GameEngine::JsonSpriteParseHelper::ParseSpriteJson(const std::string & path)
{
	ifstream stream(path);
	Json::Value root;
	stream >> root;

	assert(root.isArray());
	Vector<SpriteInfo> spriteInfos;
	for (Json::ArrayIndex i = 0; i < root.size(); ++i)
	{
		spriteInfos.PushBack(ParseMembers(root[i]));
	}

	return spriteInfos;
}

SpriteInfo GameEngine::JsonSpriteParseHelper::ParseMembers(Json::Value & value)
{
	assert(value.isObject());
	SpriteInfo spriteInfo;
	for (const auto& member : value.getMemberNames())
	{
		if (member == PathElementName)
		{
			spriteInfo.mName = value[member].asString();
		}
		else if (member == SpriteElementName)
		{
			spriteInfo.mSprites = ParseArray(value[member]);
		}
		else if (member == "size")
		{
			spriteInfo.mSize = value[member].asInt();
		}
	}
	return spriteInfo;
}

Vector<std::pair<int, int>> GameEngine::JsonSpriteParseHelper::ParseArray(Json::Value & value)
{
	assert(value.isArray());
	Vector<std::pair<int, int>> sprites;
	for (Json::ArrayIndex i = 0; i < value.size(); i++)
	{
		std::pair<int, int> pair;
		for (const auto& member : value[i].getMemberNames())
		{
			if (member == RowElementName)
			{
				pair.first = value[i][member].asInt();
			}
			else if (member == ColumnElementName)
			{
				pair.second = value[i][member].asInt();
			}
		}
		sprites.PushBack(pair);
	}

	return sprites;
}
