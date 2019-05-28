#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "World.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"
#include "ActionListIf.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "ActionAdd.h"
#include "ActionExpression.h"
#include "ActionLoadJson.h"

using namespace GameEngine;
using namespace std;
using namespace std::string_literals;
using namespace glm;

namespace UnitTestLibraryDesktop
{
	#define REGISTER_TYPE(_type, _parentType) AttributedTypeManager::RegisterType(_type::TypeIdClass(), _type::Signatures(), _parentType::TypeIdClass());

	TEST_MODULE_INITIALIZE(ModuleInit)
	{
		REGISTER_TYPE(AttributedFoo, Attributed);
		REGISTER_TYPE(World, Attributed);
		REGISTER_TYPE(Sector, Attributed);
		REGISTER_TYPE(Entity, Attributed);
		REGISTER_TYPE(Action, Attributed);
		REGISTER_TYPE(ActionList, Action);
		REGISTER_TYPE(ActionListIf, ActionList);
		REGISTER_TYPE(ActionIncrement, Action);
		REGISTER_TYPE(ActionCreateAction, Action);
		REGISTER_TYPE(ActionDestroyAction, Action);
		REGISTER_TYPE(EventMessageAttributed, Attributed);
		REGISTER_TYPE(Reaction, ActionList);
		REGISTER_TYPE(ReactionAttributed, Reaction);
		REGISTER_TYPE(ActionEvent, Action);
		REGISTER_TYPE(ActionAdd, Action);
		REGISTER_TYPE(ActionExpression, Action);
		REGISTER_TYPE(ActionLoadJson, Action);

		AttributedTypeManager::FinalizeSignature();
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		AttributedTypeManager::UnregisterAllTypes();
	}
}