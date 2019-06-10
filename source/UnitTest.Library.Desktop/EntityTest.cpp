#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Avatar.h"
#include "WorldState.h"
#include "GameClock.h"
#include "GameTime.h"
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
	TEST_CLASS(EntityTest)
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

#define INIT()																			   \
		EntityFactory ef;																   \
		SectorFactory sf;																   \
		AvatarFactory af;																   \
		AttributedTypeManager::RegisterType(Avatar::TypeIdClass(), Avatar::Signatures(), Entity::TypeIdClass()); \
		AttributedTypeManager::FinalizeSignature();

#define UNINIT() AttributedTypeManager::UnregisterType(Avatar::TypeIdClass());

		TEST_METHOD(TestGameTime)
		{
			GameTime time;
			GameClock clock;
			World world(time);

			clock.Reset();
			auto startTime = clock.StartTime();
			auto lastTime = clock.LastTime();
			clock.UpdateGameTime(time);
			auto currentTime = clock.CurrentTime();
			Assert::IsTrue(clock.StartTime() == startTime);
			Assert::IsTrue(currentTime >= lastTime);
			Assert::IsTrue(time.CurrentTime() == currentTime);
			Assert::IsTrue(time.ElapsedGameTime().count() == 0);
			Assert::IsTrue(time.ElapsedGameTimeSeconds().count() >= 0.f);
			Assert::IsTrue(time.TotalGameTime().count() >= 0);
			Assert::IsTrue(time.TotalGameTimeSeconds().count() >= 0.f);
		}

		TEST_METHOD(TestEntityCopy)
		{
			Entity entity;
			entity.SetName("ABC");
			Assert::IsTrue(&const_cast<const Entity&>(entity).Actions() == &entity.Actions());

			{
				Entity copy = entity;
				Assert::AreEqual("ABC"s, copy.Name());
				Assert::AreEqual("ABC"s, copy["Name"].AsString());
			}
			
			{
				Entity copy;
				copy.SetName("AA");

				Assert::AreNotEqual("ABC"s, copy.Name());
				copy = entity;
				Assert::AreEqual("ABC"s, copy.Name());
				Assert::AreEqual("ABC"s, copy["Name"].AsString());
			}

			{
				Scope* copy = entity.Clone();
				Assert::IsTrue(copy->Is("Entity"));
				Assert::AreEqual("ABC"s, static_cast<Entity*>(copy)->Name());
				Assert::AreEqual("ABC"s, static_cast<Entity&>(*copy)["Name"].AsString());
				delete copy;
			}
		}

		TEST_METHOD(TestEntityMove)
		{
			{
				Entity entity;
				entity.SetName("ABC");
				Entity copy = move(entity);
				Assert::AreEqual("ABC"s, copy.Name());
				Assert::AreEqual("ABC"s, copy["Name"].AsString());
			}

			{
				Entity entity;
				entity.SetName("ABC");
				Entity copy;
				copy.SetName("AA");
				Assert::AreNotEqual("ABC"s, copy.Name());
				copy = move(entity);
				Assert::AreEqual("ABC"s, copy.Name());
				Assert::AreEqual("ABC"s, copy["Name"].AsString());
			}
		}

		TEST_METHOD(TestEntitySetSector)
		{
			Sector sec1;
			Sector sec2;
			Entity* entity = new Entity();
			Assert::IsNull(entity->GetParent());
			Assert::IsNull(entity->GetParent<Sector>());

			entity->SetSector(sec1);
			Assert::IsTrue(&sec1 == entity->GetParent<Sector>());
			Assert::IsTrue(&sec1["Entities"][0] == entity);

			entity->SetSector(sec2);
			Assert::IsTrue(&sec2 == entity->GetParent<Sector>());
			Assert::AreEqual(0_z, sec1["Entities"].Size());
			Assert::IsTrue(&sec2["Entities"][0] == entity);
		}

		TEST_METHOD(TestEntityUpdate)
		{
			Entity entity;
			GameTime time;
			WorldState state(time);
			Assert::IsNull(state.GetEntity());
			entity.Update(state);
			Assert::IsTrue(state.GetEntity() == &entity);
		}

		TEST_METHOD(TestSectorCopy)
		{
			INIT();

			Sector sector;
			Entity* entity = new Entity();
			Avatar* avatar = new Avatar();
			sector.SetName("Sector");
			entity->SetSector(sector);
			avatar->SetSector(sector);

			{
				Sector copy = sector;
				Assert::AreEqual("Sector"s, copy.Name());
				Assert::AreEqual("Sector"s, copy["Name"].AsString());
				Assert::IsTrue(copy.Entities()[0].Is("Entity") && !copy.Entities()[0].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[0] != entity);
				Assert::IsTrue(copy.Entities()[1].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[1] != avatar);
			}

			{
				Sector copy;
				Avatar* copyAvatar = new Avatar();
				copy.SetName("AA");
				copyAvatar->SetSector(copy);

				copy = sector;
				Assert::AreEqual("Sector"s, copy.Name());
				Assert::AreEqual("Sector"s, copy["Name"].AsString());
				Assert::IsTrue(copy.Entities()[0].Is("Entity") && !copy.Entities()[0].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[0] != entity);
				Assert::IsTrue(copy.Entities()[1].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[1] != avatar);
			}

			{
				Sector& copy = *static_cast<Sector*>(sector.Clone());
				Assert::AreEqual("Sector"s, copy.Name());
				Assert::AreEqual("Sector"s, copy["Name"].AsString());
				Assert::IsTrue(copy.Entities()[0].Is("Entity") && !copy.Entities()[0].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[0] != entity);
				Assert::IsTrue(copy.Entities()[1].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[1] != avatar);
				delete &copy;
			}

			UNINIT();
		}

		TEST_METHOD(TestSectorMove)
		{
			INIT();

			{
				Sector sector;
				Entity* entity = new Entity();
				Avatar* avatar = new Avatar();
				sector.SetName("Sector");
				entity->SetSector(sector);
				avatar->SetSector(sector);
				Sector copy = move(sector);
				Assert::AreEqual("Sector"s, copy.Name());
				Assert::AreEqual("Sector"s, copy["Name"].AsString());
				Assert::IsTrue(copy.Entities()[0].Is("Entity") && !copy.Entities()[0].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[0] == entity);
				Assert::IsTrue(copy.Entities()[1].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[1] == avatar);
			}

			{
				Sector sector;
				Entity* entity = new Entity();
				Avatar* avatar = new Avatar();
				sector.SetName("Sector");
				entity->SetSector(sector);
				avatar->SetSector(sector);
				Sector copy;
				Avatar* copyAvatar = new Avatar();
				copy.SetName("AA");
				copyAvatar->SetSector(copy);

				copy = move(sector);
				Assert::AreEqual("Sector"s, copy.Name());
				Assert::AreEqual("Sector"s, copy["Name"].AsString());
				Assert::IsTrue(copy.Entities()[0].Is("Entity") && !copy.Entities()[0].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[0] == entity);
				Assert::IsTrue(copy.Entities()[1].Is("Avatar"));
				Assert::IsTrue(&copy.Entities()[1] == avatar);
			}

			UNINIT();
		}

		TEST_METHOD(TestSectorSetWorld)
		{
			GameTime time;
			World world(time);
			World world2(time);
			Sector* sector = new Sector();
			Assert::IsTrue(sector->GetParent() == nullptr);
			sector->SetWorld(world);
			Assert::IsTrue(sector->GetParent<World>() == &world);
			Assert::IsTrue(&world.Sectors()[0] == sector);
			sector->SetWorld(world2);
			Assert::IsTrue(sector->GetParent<World>() == &world2);
			Assert::AreEqual(0_z, world.Sectors().Size());
			Assert::IsTrue(&world2.Sectors()[0] == sector);
		}

		TEST_METHOD(TestSectorUpdate)
		{
			INIT();

			Sector sector;
			Entity* entity = new Entity();
			Avatar* avatar = new Avatar();
			entity->SetSector(sector);
			GameTime time;
			WorldState state(time);

			Assert::IsNull(state.GetSector());
			Assert::IsNull(state.GetEntity());
			sector.Update(state);
			Assert::IsTrue(state.GetSector() == &sector);
			Assert::IsTrue(state.GetEntity() == entity);
			avatar->SetSector(sector);
			sector.Update(state);
			Assert::IsTrue(state.GetEntity() == avatar);

			UNINIT();
		}

		TEST_METHOD(TestCreateEntity)
		{
			INIT();
			Sector sector;

			sector.CreateEntity("Entity", "Entity1");
			Scope* scope = &sector.Entities()[0];
			const_cast<const Sector&>(sector).Entities();
			Assert::IsTrue(scope->Is("Entity"));
			Assert::AreEqual("Entity1"s, scope->Find("Name")->AsString());
			Assert::IsTrue(&sector == scope->GetParent<Sector>());

			sector.CreateEntity("Entity", "Entity2");
			scope = &sector.Entities()[1];
			Assert::IsTrue(scope->Is("Entity"));
			Assert::AreEqual("Entity2"s, scope->Find("Name")->AsString());
			Assert::IsTrue(&sector == scope->GetParent<Sector>());

			sector.CreateEntity("Avatar", "A");
			scope = &sector.Entities()[2];
			Assert::IsTrue(scope->Is("Avatar"));
			Assert::AreEqual("A"s, scope->Find("Name")->AsString());
			Assert::IsTrue(&sector == scope->GetParent<Sector>());

			UNINIT();
		}

		TEST_METHOD(TestWorldCopy)
		{
			INIT();
			GameTime time;
			World world(time);
			world.SetName("BigWorld");
			Sector* s1 = world.CreateSector("Sector1");
			Sector* s2 = world.CreateSector("Sector2");
			s1->CreateEntity("Avatar", "Avatar1");

			{
				World copy = world;
				Assert::AreEqual("BigWorld"s, copy.Name());
				Assert::AreEqual(2_z, copy.Sectors().Size());
				Assert::IsTrue(copy.Sectors()[0].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[0] != s1);
				Assert::IsTrue(copy.Sectors()[1].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[1] != s2);
				Assert::IsTrue(copy.Sectors()[0]["Entities"][0].Is("Avatar"));
				Assert::AreEqual("Avatar1"s, copy.Sectors()[0]["Entities"][0]["Name"].AsString());
			}

			{
				World copy(time);
				copy.CreateSector("S");
				copy = world;
				Assert::AreEqual("BigWorld"s, copy.Name());
				Assert::AreEqual(2_z, copy.Sectors().Size());
				Assert::IsTrue(copy.Sectors()[0].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[0] != s1);
				Assert::IsTrue(copy.Sectors()[1].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[1] != s2);
				Assert::IsTrue(copy.Sectors()[0]["Entities"][0].Is("Avatar"));
				Assert::AreEqual("Avatar1"s, copy.Sectors()[0]["Entities"][0]["Name"].AsString());
			}

			{
				Scope* scope = world.Clone();
				World& copy = static_cast<World&>(*scope);
				Assert::AreEqual("BigWorld"s, copy.Name());
				Assert::AreEqual(2_z, copy.Sectors().Size());
				Assert::IsTrue(copy.Sectors()[0].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[0] != s1);
				Assert::IsTrue(copy.Sectors()[1].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[1] != s2);
				Assert::IsTrue(copy.Sectors()[0]["Entities"][0].Is("Avatar"));
				Assert::AreEqual("Avatar1"s, copy.Sectors()[0]["Entities"][0]["Name"].AsString());
				delete scope;
			}

			UNINIT();
		}

		TEST_METHOD(TestWorldMove)
		{
			INIT();
			GameTime time;
			
			{
				World world(time);
				world.SetName("BigWorld");
				Sector* s1 = world.CreateSector("Sector1");
				Sector* s2 = world.CreateSector("Sector2");
				s1->CreateEntity("Avatar", "Avatar1");
				World copy = move(world);
				Assert::AreEqual("BigWorld"s, copy.Name());
				Assert::AreEqual(2_z, copy.Sectors().Size());
				Assert::IsTrue(copy.Sectors()[0].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[0] == s1);
				Assert::IsTrue(copy.Sectors()[1].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[1] == s2);
				Assert::IsTrue(copy.Sectors()[0]["Entities"][0].Is("Avatar"));
				Assert::AreEqual("Avatar1"s, copy.Sectors()[0]["Entities"][0]["Name"].AsString());
			}

			{
				World world(time);
				world.SetName("BigWorld");
				Sector* s1 = world.CreateSector("Sector1");
				Sector* s2 = world.CreateSector("Sector2");
				s1->CreateEntity("Avatar", "Avatar1");
				World copy(time);
				copy.CreateSector("S");
				copy = move(world);
				Assert::AreEqual("BigWorld"s, copy.Name());
				Assert::AreEqual(2_z, copy.Sectors().Size());
				Assert::IsTrue(copy.Sectors()[0].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[0] == s1);
				Assert::IsTrue(copy.Sectors()[1].Is("Sector"));
				Assert::IsTrue(&copy.Sectors()[1] == s2);
				Assert::IsTrue(copy.Sectors()[0]["Entities"][0].Is("Avatar"));
				Assert::AreEqual("Avatar1"s, copy.Sectors()[0]["Entities"][0]["Name"].AsString());
			}

			UNINIT();
		}

		TEST_METHOD(TestCreateSector)
		{
			INIT();
			GameTime time;
			World world(time);

			Sector* s1 = world.CreateSector("Sector1");
			Assert::IsTrue(&world.Sectors()[0] == s1);
			Assert::IsTrue(s1->GetParent<World>() == &world);

			Sector* s2 = world.CreateSector("Sector2");
			Assert::IsTrue(&world.Sectors()[1] == s2);
			Assert::IsTrue(s2->GetParent<World>() == &world);

			Sector* s3 = world.CreateSector("Sector3");
			Assert::IsTrue(&world.Sectors()[2] == s3);
			Assert::IsTrue(s3->GetParent<World>() == &world);

			Assert::IsTrue(&const_cast<const World&>(world).Sectors()[0] == s1);

			UNINIT();
		}

		TEST_METHOD(TestWorldUpdate)
		{
			INIT();
			GameTime time;
			WorldState state(time);
			World world(time);
			world.CreateSector("Sector1");
			world.CreateSector("Sector2");
			Sector* s3 = world.CreateSector("Sector3");
			s3->CreateEntity("Entity", "Entity1");
			s3->CreateEntity("Entity", "Entity2");
			Avatar* e3 = static_cast<Avatar*>(s3->CreateEntity("Avatar", "Avatar1"));

			world.Update(state);
			Assert::IsTrue(state.GetWorld() == &world);
			Assert::IsTrue(state.GetSector() == s3);
			Assert::IsTrue(state.GetEntity() == e3);
			Assert::AreEqual(1_z, e3->mTickCount);

			UNINIT();
		}

		TEST_METHOD(TestIntegration)
		{
			INIT();
			GameTime time;
			GameClock clock;
			shared_ptr<World> world = make_shared<World>(time);
			JsonTableParseHelper helper;
			JsonTableParseHelper::SharedData data(world);
			JsonParseMaster master(data);
			master.AddHelper(helper);
			Assert::IsTrue(master.ParseFromFile("content/EntityJsonTest.json"));
			Avatar* e3 = static_cast<Avatar*>(&(*world)["Sectors"][1]["Entities"][2]);

			size_t count = 0;
			clock.Reset();
			while (e3->mTickCount < 1000)
			{
				clock.UpdateGameTime(time);
				int64_t gameTime = time.TotalGameTime().count();
				int64_t frameTime = time.ElapsedGameTime().count();
				world->Update();
				Assert::AreEqual(++count, e3->mTickCount);
				Assert::AreEqual(gameTime, e3->mGameTime);
				Assert::AreEqual(frameTime, e3->mFrameTime);
			}

			UNINIT();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EntityTest::sStartMemState;
}