#include "pch.h"
#include "CppUnitTest.h"
#include "Transform.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace glm;
using namespace GameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
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
	TEST_CLASS(TransformTest)
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

		TEST_METHOD(TestConstructor)
		{
			{
				Transform trans;
				Assert::AreEqual(vec4(), trans.GetPosition());
				Assert::AreEqual(vec4(), trans.GetRotation());
				Assert::AreEqual(vec4(1), trans.GetScale());
				Assert::AreEqual(mat4(1.f), trans.GetMatrix());
			}

			{
				Transform trans(vec4(1, 2, 3, 1), vec4(0, 0, 0, 0), vec4(1, 2, 1, 1));
				Assert::AreEqual(vec4(1, 2, 3, 1), trans.GetPosition());
				Assert::AreEqual(vec4(0.f), trans.GetRotation());
				Assert::AreEqual(vec4(1, 2, 1, 1), trans.GetScale());
				Assert::AreEqual(mat4(1, 0, 0, 0,
									  0, 2, 0, 0,
									  0, 0, 1, 0,
									  1, 2, 3, 1), trans.GetMatrix());
			}
		}

		TEST_METHOD(TestApplyTransform)
		{
			vec4 point(0, 0, 0, 1);
			Transform trans(vec4(0.f), vec4(0.f), vec4(0.f));
			Assert::AreEqual(mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1), trans.GetMatrix());
			point = trans * point;
			Assert::AreEqual(vec4(0, 0, 0, 1), point);

			trans.SetPosition(5, 0, 0);
			trans.SetRotation(0, 0, 90);
			trans.SetScale(2, 2, 1);
			point = trans * point;
			Assert::IsTrue(Transform::Equal(point, vec4(5, 0, 0, 1)));
			point = vec4(0, 0, 0, 1);
			point = trans * Transform(vec4(3, 0, 0, 1), vec4(0, 0, 0, 1), vec4(1)) * point;
			Assert::IsTrue(Transform::Equal(point, vec4(5, 6, 0, 1)));

			{
				point = vec4(0, 0, 0, 1);
				trans.SetPosition(1, 0, 0);
				trans.SetRotation(0, 90, 0);
				trans.SetScale(2, 1, 1);
				point = trans * point;
				Assert::IsTrue(Transform::Equal(point, vec4(1, 0, 0, 1)));
			}

			{
				point = vec4(0, 0, 0, 1);
				Transform trans2(vec4(5, 0, 0, 1), vec4(90, 0, 0, 1), vec4(2.f));
				point = trans2 * Transform(vec4(1, 2, 0, 1), vec4(0, 0, 180, 1), vec4(2)) * point;
				Assert::IsTrue(Transform::Equal(point, vec4(7, 0, 4, 1)));
			}


			{
				point = vec4(0, 0, 0, 1);
				Transform trans1(vec4(2, 2, 0, 1), vec4(0.f), vec4(1.f));
				Transform trans2 = Transform(vec4(0, 0, 3, 1), vec4(180, 0, 90, 1), vec4(1, 5, 2, 1));
				Transform trans3(vec4(0, 3, 0, 1), vec4(0.f), vec4(1.f));
				point = trans1 * trans2 * trans3 * point;
				Assert::IsTrue(Transform::Equal(point, vec4(-13, 2, 3, 1)));
			}

			{
				point = vec4(0, 0, 0, 1);
				Transform trans1(vec4(0, 0, 0, 1), vec4(0, 0, 90, 1), vec4(2, 2, 3, 1));
				Transform trans2(vec4(1, 2, 3, 1), vec4(90, 0, 90, 1), vec4(3, 2, 1, 1));
				Transform trans3(vec4(1, 2, 3, 1), vec4(0), vec4(1));
				point = trans1 * trans2 * point;
				Assert::IsTrue(Transform::Equal(point, vec4(-4, 2, 9, 1)));
				Transform finalTrans = trans1 * trans2 * trans3;
				point = finalTrans * vec4(0, 0, 0, 1);
				Assert::IsTrue(Transform::Equal(point, vec4(2, -6, 18, 1)));
			}
		}

		TEST_METHOD(TestBreakMatrix)
		{
			{
				Transform trans(mat4(1.f));
				Assert::AreEqual(vec4(0, 0, 0, 1), trans.GetPosition());
				Assert::AreEqual(vec4(0, 0, 0, 1), trans.GetRotation());
				Assert::AreEqual(vec4(1, 1, 1, 1), trans.GetScale());
			}

			{
				Transform trans(Transform(vec4(1, 2, 3, 1), vec4(45, 60, 30, 1), vec4(4, 5, 6, 1)).GetMatrix());
				Assert::IsTrue(Transform::Equal(trans.GetPosition(), vec4(1, 2, 3, 1)));
				Assert::IsTrue(Transform::Equal(trans.GetRotation(), vec4(45, 60, 30, 1)));
				Assert::IsTrue(Transform::Equal(trans.GetScale(), vec4(4, 5, 6, 1)));
			}

			{
				Transform trans;
				trans.SetPosition(1, 2, 3);
				trans.SetRotation(10, 20, 30);
				trans.SetScale(3, 2, 1);
				Transform trans2 = trans * Transform(vec4(), vec4(0, 0, 0, 1), vec4(1));
				Assert::IsTrue(Transform::Equal(trans2.GetPosition(), vec4(1, 2, 3, 1)));
				Assert::IsTrue(Transform::Equal(trans2.GetRotation(), vec4(10, 20, 30, 1)));
				Assert::IsTrue(Transform::Equal(trans.GetScale(), vec4(3, 2, 1, 1)));

				trans2.SetPosition(trans2.GetPosition() + vec4(1, 2, 3, 0));
				trans2.SetRotation(trans2.GetRotation() + vec4(10, 20, 30, 0));
				trans2.SetScale(trans2.GetScale() + vec4(3, 2, 1, 0));
				Assert::IsTrue(Transform::Equal(trans2.GetPosition(), vec4(2, 4, 6, 1)));
				Assert::IsTrue(Transform::Equal(trans2.GetRotation(), vec4(20, 40, 60, 1)));
				Assert::IsTrue(Transform::Equal(trans2.GetScale(), vec4(6, 4, 2, 1)));

				Transform trans3 = trans2 * Transform();
				Assert::IsTrue(Transform::Equal(trans3.GetPosition(), vec4(2, 4, 6, 1)));
				Assert::IsTrue(Transform::Equal(trans3.GetRotation(), vec4(20, 40, 60, 1)));
				Assert::IsTrue(Transform::Equal(trans3.GetScale(), vec4(6, 4, 2, 1)));
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState TransformTest::sStartMemState;
}