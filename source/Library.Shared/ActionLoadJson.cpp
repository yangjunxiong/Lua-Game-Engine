#include "pch.h"
#include "ActionLoadJson.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "Sector.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionLoadJson);

ActionLoadJson::ActionLoadJson() :
	Action(ActionLoadJson::TypeIdClass())
{}

void ActionLoadJson::Update(WorldState& state)
{
	assert(!mClass.empty());
	assert(!mParentKey.empty());
	assert(!mJsonFile.empty());
	Action::Update(state);

	// Create the class and parse from json
	Scope* scope = Factory<Scope>::Create(mClass);
	shared_ptr<Scope> ptr(scope, [](Scope*) {});
	JsonTableParseHelper::SharedData data(ptr);
	JsonParseMaster master(data);
	JsonTableParseHelper helper;
	master.AddHelper(helper);
	bool success = master.ParseFromFile(mJsonFile);
	success;
	assert(("ActionLoadJson: Parsing from file fails", success));

	// Find parent and attach the parsed class to it
	auto[datum, parentScope] = Search(mParentKey);
	assert(("ActionLoadJson: Finding parent fails", datum != nullptr && parentScope != nullptr));
	parentScope->Adopt(*scope, parentScope->FindKey(*datum));
	assert(scope->GetParent() == parentScope);
	if (scope->Is(Sector::TypeIdClass()))
	{
		static_cast<Sector&>(*scope).Start(state);
	}
	else if (scope->Is(Entity::TypeIdClass()))
	{
		static_cast<Entity&>(*scope).Start(state);
	}
}

gsl::owner<ActionLoadJson*> ActionLoadJson::Clone() const
{
	return new ActionLoadJson(*this);
}

const std::string& ActionLoadJson::GetLoadClass() const
{
	return mClass;
}

const std::string& ActionLoadJson::GetLoadParentKey() const
{
	return mParentKey;
}

const std::string& ActionLoadJson::GetJsonFileName() const
{
	return mJsonFile;
}

void ActionLoadJson::SetLoadClass(const std::string& name)
{
	mClass = name;
}

void ActionLoadJson::SetLoadParentKey(const std::string& name)
{
	mParentKey = name;
}

void ActionLoadJson::SetJsonFileName(const std::string& name)
{
	mJsonFile = name;
}

const Vector<Attributed::Signature> ActionLoadJson::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Class", Datum::DatumType::String, 1, offsetof(ActionLoadJson, mClass)),
		Signature("Parent", Datum::DatumType::String, 1, offsetof(ActionLoadJson, mParentKey)),
		Signature("File", Datum::DatumType::String, 1, offsetof(ActionLoadJson, mJsonFile))
	});
}