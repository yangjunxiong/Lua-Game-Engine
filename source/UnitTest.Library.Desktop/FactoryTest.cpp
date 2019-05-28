#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "AttributedFoo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;
using namespace std;
using namespace std::string_literals;
using namespace glm;

DECLARE_FACTORY(Scope, RTTI);

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTest)
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

		TEST_METHOD(TestFactoryMacro)
		{
			{
				FooFactory fooFactory;
				Assert::AreEqual("Foo"s, fooFactory.ClassName());
				Foo* foo = fooFactory.Create();
				Assert::IsTrue(foo->Is("Foo"));
				delete foo;
			}

			{
				AttributedFooFactory attributedFooFactory;
				Assert::AreEqual("AttributedFoo"s, attributedFooFactory.ClassName());
				Foo* foo = attributedFooFactory.Create();
				Assert::IsTrue(foo->Is("AttributedFoo"));
				delete foo;
			}

			{
				ScopeFactory scopeFactory;
				Assert::AreEqual("Scope"s, scopeFactory.ClassName());
				RTTI* scope = scopeFactory.Create();
				Assert::IsTrue(scope->Is("Scope"));
				delete scope;
			}
		}

		TEST_METHOD(TestAddRemoveFactory)
		{
			Assert::IsNull(Factory<Foo>::Find("Foo"));
			FooFactory* fooFactory = new FooFactory();
			Assert::IsNotNull(Factory<Foo>::Find("Foo"));
			Assert::IsTrue(Factory<Foo>::Find("Foo") == fooFactory);
			Assert::ExpectException<exception>([] { FooFactory factory; });

			Assert::IsNull(Factory<Foo>::Find("AttributedFoo"));
			AttributedFooFactory* attributedFooFactory = new AttributedFooFactory();
			Assert::IsNotNull(Factory<Foo>::Find("AttributedFoo"));
			Assert::IsTrue(Factory<Foo>::Find("AttributedFoo") == attributedFooFactory);
			Assert::ExpectException<exception>([attributedFooFactory] { AttributedFooFactory factory; });

			Assert::IsNull(Factory<RTTI>::Find("Foo"));
			Assert::IsNull(Factory<RTTI>::Find("AttributedFoo"));
			ScopeFactory* scopeFactory = new ScopeFactory();
			Assert::IsTrue(Factory<RTTI>::Find("Scope") == scopeFactory);
			Assert::IsNull(Factory<Foo>::Find("Scope"));

			delete fooFactory;
			delete attributedFooFactory;
			delete scopeFactory;
			Assert::IsNull(Factory<Foo>::Find("Foo"));
			Assert::IsNull(Factory<Foo>::Find("AttributedFoo"));
			Assert::IsNull(Factory<RTTI>::Find("Scope"));
		}

		TEST_METHOD(TestUseFactory)
		{
			{
				Foo* foo = Factory<Foo>::Create("Foo");
				Assert::IsNull(foo);
				foo = Factory<Foo>::Create("AttributedFoo");
				Assert::IsNull(foo);

				FooFactory fooFactory;
				foo = Factory<Foo>::Create("Foo");
				Assert::IsNotNull(foo);
				Assert::IsTrue(foo->Is("Foo"));
				delete foo;

				foo = Factory<Foo>::Create("AttributedFoo");
				Assert::IsNull(foo);
				AttributedFooFactory attributedFooFactory;
				foo = Factory<Foo>::Create("AttributedFoo");
				Assert::IsNotNull(foo);
				Assert::IsTrue(foo->Is("AttributedFoo"));
				delete foo;
			}

			Assert::IsNull(Factory<Foo>::Create("Foo"));
			Assert::IsNull(Factory<Foo>::Create("AttributedFoo"));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState FactoryTest::sStartMemState;
}