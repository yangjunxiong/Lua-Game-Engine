#pragma once
#include "IJsonParseHelper.h"
#include "SpriteSheet.h"
#include "vector.h"
#include <string>
#include <memory>

namespace GameEngine
{
	struct SpriteInfo
	{
		std::string mName;
		Vector<std::pair<int, int>> mSprites;
		size_t mSize = 1;
	};

	class JsonSpriteParseHelper
	{
	public:
		Vector<SpriteInfo> ParseSpriteJson(const std::string& path);

		JsonSpriteParseHelper() = default;
		~JsonSpriteParseHelper() = default;

	private:
		inline static const std::string PathElementName = "name";
		inline static const std::string SpriteElementName = "sprite";
		inline static const std::string RowElementName = "row";
		inline static const std::string ColumnElementName = "col";

		SpriteInfo ParseMembers(Json::Value& value);
		Vector<std::pair<int, int>> ParseArray(Json::Value& value);
	};
}