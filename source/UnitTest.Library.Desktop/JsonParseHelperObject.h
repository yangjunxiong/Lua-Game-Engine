#pragma once
#include "IJsonParseHelper.h"
#include "vector.h"

namespace GameEngine
{
	class Scope;

	class JsonParseHelperObject : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonParseHelperObject, IJsonParseHelper);

	public:
		class SharedData : public BaseSharedData
		{
			RTTI_DECLARATIONS(JsonParseHelperObject::SharedData, BaseSharedData);

		public:
			SharedData();
			~SharedData();
			virtual gsl::owner<BaseSharedData*> Create() const override;
			Scope* mScope = nullptr;
		};
		
		Vector<Scope*> mStack;

		virtual void Initialize() override;
		virtual bool StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		virtual bool EndHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		virtual gsl::owner<IJsonParseHelper*> Create() const override;
	};
}
