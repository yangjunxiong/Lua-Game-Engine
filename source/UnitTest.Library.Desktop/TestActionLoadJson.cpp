#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "JsonParseHelperVector.h"
#include "JsonParseHelperInteger.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Avatar.h"
#include "GameTime.h"
#include "ActionIncrement.h"
#include "ActionLoadJson.h"
#include <json/json.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;
using namespace std;
using namespace std::string_literals;
using namespace glm;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> inline std::wstring ToString<Datum::DatumType>(const Datum::DatumType& type)
	{
		RETURN_WIDE_STRING(static_cast<int>(type));
	}

	template<> inline std::wstring ToString<glm::vec4>(const glm::vec4& vec)
	{
		RETURN_WIDE_STRING(vec.x);
	}

	template<> inline std::wstring ToString<glm::mat4>(const glm::mat4& mat)
	{
		RETURN_WIDE_STRING(mat[0][0]);
	}
}

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionLoadJsonTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(TestLoadJson)
		{
			ActionLoadJsonFactory f;
			EntityFactory f2;
			SectorFactory f3;
			Entity entity;
			entity.AppendAuxiliaryAttribute("Hold").SetType(Datum::DatumType::Table);
			ActionLoadJson* load = static_cast<ActionLoadJson*>(entity.CreateAction("ActionLoadJson", "Load"));
			load->SetLoadClass("Entity");
			load->SetLoadParentKey("Hold");
			load->SetJsonFileName("content/LoadJsonTest.json");
			GameTime time;
			WorldState state(time);

			// Load child entity once
			entity.Update(state);
			Assert::AreEqual(1_z, entity["Hold"].Size());
			{
				Entity& child = static_cast<Entity&>(entity["Hold"].AsTable(0));
				Assert::AreEqual("Child"s, child.Name());
				Assert::AreEqual(mat4(3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1), child.GetTransform().GetMatrix());
				Assert::AreEqual(1_z, child.Actions().Size());
				ActionLoadJson& childLoad = static_cast<ActionLoadJson&>(child.Actions()[0]);
				Assert::AreEqual("AnotherLoad"s, childLoad.Name());
				Assert::AreEqual("Entity"s, childLoad.GetLoadClass());
				Assert::AreEqual("AnotherHold"s, childLoad.GetLoadParentKey());
				Assert::AreEqual("content/LoadJsonTest.json"s, childLoad.GetJsonFileName());
			}
			
			// Load child entity twice
			entity.Update(state);
			{
				Entity& child = static_cast<Entity&>(entity["Hold"].AsTable(1));
				Assert::AreEqual("Child"s, child.Name());
				Assert::AreEqual(mat4(3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1), child.GetTransform().GetMatrix());
				Assert::AreEqual(1_z, child.Actions().Size());
				ActionLoadJson& childLoad = static_cast<ActionLoadJson&>(child.Actions()[0]);
				Assert::AreEqual("AnotherLoad"s, childLoad.Name());
				Assert::AreEqual("Entity"s, childLoad.GetLoadClass());
				Assert::AreEqual("AnotherHold"s, childLoad.GetLoadParentKey());
				Assert::AreEqual("content/LoadJsonTest.json"s, childLoad.GetJsonFileName());
			}

			// Load nested child entity with the action in child entity
			Entity& child = static_cast<Entity&>(entity["Hold"].AsTable(1));
			child.Update(state);
			Assert::AreEqual(1_z, child["AnotherHold"].Size());
			Entity& nestedChild = static_cast<Entity&>(child["AnotherHold"][0]);
			Assert::AreEqual("Child"s, nestedChild.Name());
			Assert::AreEqual(1_z, nestedChild.Actions().Size());
			ActionLoadJson& childLoad = static_cast<ActionLoadJson&>(nestedChild.Actions()[0]);
			Assert::AreEqual("AnotherLoad"s, childLoad.Name());
			Assert::AreEqual("Entity"s, childLoad.GetLoadClass());
			Assert::AreEqual("AnotherHold"s, childLoad.GetLoadParentKey());
			Assert::AreEqual("content/LoadJsonTest.json"s, childLoad.GetJsonFileName());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionLoadJsonTest::sStartMemState;
}