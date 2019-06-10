#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Avatar.h"
#include "WorldState.h"
#include "GameTime.h"
#include "Action.h"
#include "ActionListIf.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionExpression.h"
#include "Tokenizer.h"
#include <json/json.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;
using namespace std;
using namespace std::string_literals;
using namespace glm;
using Token = Tokenizer::Token;
using TokenType = Tokenizer::TokenType;

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

	template<> inline std::wstring ToString<TokenType>(const TokenType& value)
	{
		RETURN_WIDE_STRING(int(value));
	}
}

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionExpressionTest)
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

		TEST_METHOD(TestTokenize)
		{
			{
				string exp = R"#(hello123 = (1 + 2.5) * "Name")#";
				Vector<Token> output;
				Tokenizer::Tokenize(exp, output);
				Assert::AreEqual(9_z, output.Size());

				Assert::AreEqual("hello123"s, output[0].String);
				Assert::AreEqual("="s, output[1].String);
				Assert::AreEqual("("s, output[2].String);
				Assert::AreEqual("1"s, output[3].String);
				Assert::AreEqual("+"s, output[4].String);
				Assert::AreEqual("2.5"s, output[5].String);
				Assert::AreEqual(")"s, output[6].String);
				Assert::AreEqual("*"s, output[7].String);
				Assert::AreEqual("Name"s, output[8].String);

				Assert::AreEqual(TokenType::Variable, output[0].Type);
				Assert::AreEqual(TokenType::Assign, output[1].Type);
				Assert::AreEqual(TokenType::LeftParen, output[2].Type);
				Assert::AreEqual(TokenType::Integer, output[3].Type);
				Assert::AreEqual(TokenType::Add, output[4].Type);
				Assert::AreEqual(TokenType::Float, output[5].Type);
				Assert::AreEqual(TokenType::RightParen, output[6].Type);
				Assert::AreEqual(TokenType::Multiply, output[7].Type);
				Assert::AreEqual(TokenType::StringLiteral, output[8].Type);
			}

			{
				string exp = "5 + ((1 + 2) * 4) - 3";
				Vector<Token> output;
				Tokenizer::Tokenize(exp, output);
				Assert::AreEqual(13_z, output.Size());

				Assert::AreEqual("5"s, output[0].String);
				Assert::AreEqual("+"s, output[1].String);
				Assert::AreEqual("("s, output[2].String);
				Assert::AreEqual("("s, output[3].String);
				Assert::AreEqual("1"s, output[4].String);
				Assert::AreEqual("+"s, output[5].String);
				Assert::AreEqual("2"s, output[6].String);
				Assert::AreEqual(")"s, output[7].String);
				Assert::AreEqual("*"s, output[8].String);
				Assert::AreEqual("4"s, output[9].String);
				Assert::AreEqual(")"s, output[10].String);
				Assert::AreEqual("-"s, output[11].String);
				Assert::AreEqual("3"s, output[12].String);

				Assert::AreEqual(TokenType::Integer, output[0].Type);
				Assert::AreEqual(TokenType::Add, output[1].Type);
				Assert::AreEqual(TokenType::LeftParen, output[2].Type);
				Assert::AreEqual(TokenType::LeftParen, output[3].Type);
				Assert::AreEqual(TokenType::Integer, output[4].Type);
				Assert::AreEqual(TokenType::Add, output[5].Type);
				Assert::AreEqual(TokenType::Integer, output[6].Type);
				Assert::AreEqual(TokenType::RightParen, output[7].Type);
				Assert::AreEqual(TokenType::Multiply, output[8].Type);
				Assert::AreEqual(TokenType::Integer, output[9].Type);
				Assert::AreEqual(TokenType::RightParen, output[10].Type);
				Assert::AreEqual(TokenType::Minus, output[11].Type);
				Assert::AreEqual(TokenType::Integer, output[12].Type);
			}

			{
				string exp = "result = (-1 - -2) * ((speed / ratio + minSpeed) * deltaTime) - 100.0";
				Vector<Token> output;
				Tokenizer::Tokenize(exp, output);
				Assert::AreEqual(23_z, output.Size());

				Assert::AreEqual("result"s, output[0].String);
				Assert::AreEqual("="s, output[1].String);
				Assert::AreEqual("("s, output[2].String);
				Assert::AreEqual("-"s, output[3].String);
				Assert::AreEqual("1"s, output[4].String);
				Assert::AreEqual("-"s, output[5].String);
				Assert::AreEqual("-"s, output[6].String);
				Assert::AreEqual("2"s, output[7].String);
				Assert::AreEqual(")"s, output[8].String);
				Assert::AreEqual("*"s, output[9].String);
				Assert::AreEqual("("s, output[10].String);
				Assert::AreEqual("("s, output[11].String);
				Assert::AreEqual("speed"s, output[12].String);
				Assert::AreEqual("/"s, output[13].String);
				Assert::AreEqual("ratio"s, output[14].String);
				Assert::AreEqual("+"s, output[15].String);
				Assert::AreEqual("minSpeed"s, output[16].String);
				Assert::AreEqual(")"s, output[17].String);
				Assert::AreEqual("*"s, output[18].String);
				Assert::AreEqual("deltaTime"s, output[19].String);
				Assert::AreEqual(")"s, output[20].String);
				Assert::AreEqual("-"s, output[21].String);
				Assert::AreEqual("100.0"s, output[22].String);

				Assert::AreEqual(TokenType::Variable, output[0].Type);
				Assert::AreEqual(TokenType::Assign, output[1].Type);
				Assert::AreEqual(TokenType::LeftParen, output[2].Type);
				Assert::AreEqual(TokenType::Negate, output[3].Type);
				Assert::AreEqual(TokenType::Integer, output[4].Type);
				Assert::AreEqual(TokenType::Minus, output[5].Type);
				Assert::AreEqual(TokenType::Negate, output[6].Type);
				Assert::AreEqual(TokenType::Integer, output[7].Type);
				Assert::AreEqual(TokenType::RightParen, output[8].Type);
				Assert::AreEqual(TokenType::Multiply, output[9].Type);
				Assert::AreEqual(TokenType::LeftParen, output[10].Type);
				Assert::AreEqual(TokenType::LeftParen, output[11].Type);
				Assert::AreEqual(TokenType::Variable, output[12].Type);
				Assert::AreEqual(TokenType::Divide, output[13].Type);
				Assert::AreEqual(TokenType::Variable, output[14].Type);
				Assert::AreEqual(TokenType::Add, output[15].Type);
				Assert::AreEqual(TokenType::Variable, output[16].Type);
				Assert::AreEqual(TokenType::RightParen, output[17].Type);
				Assert::AreEqual(TokenType::Multiply, output[18].Type);
				Assert::AreEqual(TokenType::Variable, output[19].Type);
				Assert::AreEqual(TokenType::RightParen, output[20].Type);
				Assert::AreEqual(TokenType::Minus, output[21].Type);
				Assert::AreEqual(TokenType::Float, output[22].Type);
			}

			{
				string exp = "value = 2 > 1 < hello == a != b <= 1 >= 2";
				Vector<Token> output;
				Tokenizer::Tokenize(exp, output);
				Assert::AreEqual(15_z, output.Size());

				Assert::AreEqual("value"s, output[0].String);
				Assert::AreEqual("="s, output[1].String);
				Assert::AreEqual("2"s, output[2].String);
				Assert::AreEqual(">"s, output[3].String);
				Assert::AreEqual("1"s, output[4].String);
				Assert::AreEqual("<"s, output[5].String);
				Assert::AreEqual("hello"s, output[6].String);
				Assert::AreEqual("=="s, output[7].String);
				Assert::AreEqual("a"s, output[8].String);
				Assert::AreEqual("!="s, output[9].String);
				Assert::AreEqual("b"s, output[10].String);
				Assert::AreEqual("<="s, output[11].String);
				Assert::AreEqual("1"s, output[12].String);
				Assert::AreEqual(">="s, output[13].String);
				Assert::AreEqual("2"s, output[14].String);

				Assert::AreEqual(TokenType::Variable, output[0].Type);
				Assert::AreEqual(TokenType::Assign, output[1].Type);
				Assert::AreEqual(TokenType::Integer, output[2].Type);
				Assert::AreEqual(TokenType::GreaterThan, output[3].Type);
				Assert::AreEqual(TokenType::Integer, output[4].Type);
				Assert::AreEqual(TokenType::LessThan, output[5].Type);
				Assert::AreEqual(TokenType::Variable, output[6].Type);
				Assert::AreEqual(TokenType::Equal, output[7].Type);
				Assert::AreEqual(TokenType::Variable, output[8].Type);
				Assert::AreEqual(TokenType::NotEqual, output[9].Type);
				Assert::AreEqual(TokenType::Variable, output[10].Type);
				Assert::AreEqual(TokenType::LessEqual, output[11].Type);
				Assert::AreEqual(TokenType::Integer, output[12].Type);
				Assert::AreEqual(TokenType::GreaterEqual, output[13].Type);
				Assert::AreEqual(TokenType::Integer, output[14].Type);
			}
		}

		TEST_METHOD(TestConvertToRPN)
		{
			{
				// 5 + ((1 + 2) * 4) - 3
				Vector<Tokenizer::Token> input = {
					Token("5", TokenType::Integer),
					Token("+", TokenType::Add),
					Token("(", TokenType::LeftParen),
					Token("(", TokenType::LeftParen),
					Token("1", TokenType::Integer),
					Token("+", TokenType::Add),
					Token("2", TokenType::Integer),
					Token(")", TokenType::RightParen),
					Token("*", TokenType::Multiply),
					Token("4", TokenType::Integer),
					Token(")", TokenType::RightParen),
					Token("-", TokenType::Minus),
					Token("3", TokenType::Integer)
				};
				Vector<Tokenizer::Token> output;
				Tokenizer::ConvertToRPN(input, output);

				Assert::AreEqual(9_z, output.Size());
				Assert::AreEqual("5"s, output[0].String);
				Assert::AreEqual("1"s, output[1].String);
				Assert::AreEqual("2"s, output[2].String);
				Assert::AreEqual("+"s, output[3].String);
				Assert::AreEqual("4"s, output[4].String);
				Assert::AreEqual("*"s, output[5].String);
				Assert::AreEqual("+"s, output[6].String);
				Assert::AreEqual("3"s, output[7].String);
				Assert::AreEqual("-"s, output[8].String);
			}
			
			{
				// value = 1 + 2.5 * b
				Vector<Tokenizer::Token> input = {
					Token("value", TokenType::Variable),
					Token("=", TokenType::Assign),
					Token("1", TokenType::Integer),
					Token("+", TokenType::Add),
					Token("2.5", TokenType::Float),
					Token("*", TokenType::Multiply),
					Token("b", TokenType::Variable)
				};
				Vector<Tokenizer::Token> output;
				Tokenizer::ConvertToRPN(input, output);

				Assert::AreEqual(7_z, output.Size());
				Assert::AreEqual("1"s, output[0].String);
				Assert::AreEqual("2.5"s, output[1].String);
				Assert::AreEqual("b"s, output[2].String);
				Assert::AreEqual("*"s, output[3].String);
				Assert::AreEqual("+"s, output[4].String);
				Assert::AreEqual("value"s, output[5].String);
				Assert::AreEqual("="s, output[6].String);
			}

			{
				// infix: result = (1 + 2) * ((speed / ratio + minSpeed) * deltaTime) - 100.0
				// postfix: 1 2 + speed ratio / minSpeed + deltaTime * * 100.0 - result =
				Vector<Tokenizer::Token> input = {
					Token("result", TokenType::Variable),
					Token("=", TokenType::Assign),
					Token("(", TokenType::LeftParen),
					Token("1", TokenType::Integer),
					Token("+", TokenType::Add),
					Token("2", TokenType::Integer),
					Token(")", TokenType::RightParen),
					Token("*", TokenType::Multiply),
					Token("(", TokenType::LeftParen),
					Token("(", TokenType::LeftParen),
					Token("speed", TokenType::Variable),
					Token("/", TokenType::Divide),
					Token("ratio", TokenType::Variable),
					Token("+", TokenType::Add),
					Token("minSpeed", TokenType::Variable),
					Token(")", TokenType::RightParen),
					Token("*", TokenType::Multiply),
					Token("deltaTime", TokenType::Variable),
					Token(")", TokenType::RightParen),
					Token("-", TokenType::Minus),
					Token("100.0", TokenType::Float)
				};
				Vector<Tokenizer::Token> output;
				Tokenizer::ConvertToRPN(input, output);

				Assert::AreEqual(15_z, output.Size());
				Assert::AreEqual("1"s, output[0].String);
				Assert::AreEqual("2"s, output[1].String);
				Assert::AreEqual("+"s, output[2].String);
				Assert::AreEqual("speed"s, output[3].String);
				Assert::AreEqual("ratio"s, output[4].String);
				Assert::AreEqual("/"s, output[5].String);
				Assert::AreEqual("minSpeed"s, output[6].String);
				Assert::AreEqual("+"s, output[7].String);
				Assert::AreEqual("deltaTime"s, output[8].String);
				Assert::AreEqual("*"s, output[9].String);
				Assert::AreEqual("*"s, output[10].String);
				Assert::AreEqual("100.0"s, output[11].String);
				Assert::AreEqual("-"s, output[12].String);
				Assert::AreEqual("result"s, output[13].String);
				Assert::AreEqual("="s, output[14].String);
			}
		}

		TEST_METHOD(TestRunExpression)
		{
			GameTime time;
			WorldState state(time);
			Entity entity;
			entity.SetName("Entity");
			entity.AppendAuxiliaryAttribute("Health") = 100.f;
			entity.AppendAuxiliaryAttribute("Shield") = 10.f;
			entity.AppendAuxiliaryAttribute("Result") = 0.f;
			entity.AppendAuxiliaryAttribute("Condition") = 0;
			ActionExpressionFactory factory;
			ActionExpression& exp = *static_cast<ActionExpression*>(entity.CreateAction("ActionExpression", "Action"));

			exp.SetExpression("Health = Health - (20.5 - Shield)");
			entity.Update(state);
			Assert::AreEqual(89.5f, entity["Health"].AsFloat());

			entity["Health"] = 100.f;
			exp.SetExpression("Result = -10 + (20.0000 * (3 - (1 - 1))) / (Shield - 5.0) + ((((Health))) * (2.0000)) - -2.512");
			entity.Update(state);
			Assert::AreEqual(204.512f, entity["Result"].AsFloat());

			exp.SetExpression("Name = Name + \" Append-\" + Name");
			entity.Update(state);
			Assert::AreEqual("Action Append-Action"s, exp.Name());

			exp.SetName("Action");
			exp.SetExpression("/Name = /Name + ./Name + Name + ../Name + \" Append \" + \" ../Name \"");
			entity.Update(state);
			Assert::AreEqual("EntityActionActionEntity Append  ../Name "s, entity.Name());

			entity.SetName("Action");
			exp.SetExpression("../Name = /Actions[0]/Name + \" Append\"");
			entity.Update(state);
			Assert::AreEqual("Action Append"s, entity.Name());

			exp.SetExpression("Condition = !(/Condition)");
			entity.Update(state);
			Assert::IsTrue(entity["Condition"]);

			exp.SetExpression("Condition = !0");
			entity.Update(state);
			Assert::IsTrue(entity["Condition"]);

			exp.SetExpression("Condition = Shield % 4");
			entity.Update(state);
			Assert::AreEqual(2, entity["Condition"].AsInt());

			// Check logic operation
			{
				exp.SetExpression("Condition = 1 == 2");
				entity.Update(state);
				Assert::IsFalse(entity["Condition"]);

				exp.SetExpression("Condition = !Condition");
				entity.Update(state);
				Assert::IsTrue(entity["Condition"]);

				exp.SetExpression("Condition = Shield + 20.0 >= 30");
				entity.Update(state);
				Assert::IsTrue(entity["Condition"]);

				exp.SetExpression("Condition = Shield + 20.0 > 30");
				entity.Update(state);
				Assert::IsFalse(entity["Condition"]);

				exp.SetExpression("Condition = Shield + 20.0 <= 30");
				entity.Update(state);
				Assert::IsTrue(entity["Condition"]);

				exp.SetExpression("Condition = Shield + 20.0 < 30");
				entity.Update(state);
				Assert::IsFalse(entity["Condition"]);

				exp.SetExpression("Condition = Shield == 10.0 && 5 + 1 < 2 * 2");
				entity.Update(state);
				Assert::IsFalse(entity["Condition"]);

				exp.SetExpression("Condition = Shield == 10.0 || 5 + 1 < 2 * 2");
				entity.Update(state);
				Assert::IsTrue(entity["Condition"]);

				exp.SetExpression("Condition = (Shield == 10.0 && 5 + 1 < 2 * 2) || Shield >= 5");
				entity.Update(state);
				Assert::IsTrue(entity["Condition"]);

				exp.SetExpression("Condition = (Shield != 10) == (10 != 5)");
				entity.Update(state);
				Assert::IsTrue(entity["Condition"]);
			}
		}

		TEST_METHOD(TestParseFromJson)
		{
			GameTime time;
			WorldState state(time);
			shared_ptr<ActionExpression> exp = make_shared<ActionExpression>();
			JsonTableParseHelper::SharedData data(exp);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			ActionExpressionFactory f;
			Assert::IsTrue(master.ParseFromFile("content/ActionExpression.json"));
			Assert::AreEqual(100, exp->Find("Value1")->AsInt());
			Assert::AreEqual(50.f, exp->Find("Value2")->AsFloat());
			Assert::AreEqual(0, exp->Find("Value3")->AsInt());
			exp->Update(state);
			Assert::IsTrue(exp->Find("Value3"));

			ActionExpression* copy = exp->Clone();
			Assert::IsTrue(copy->Find("Value3"));
			Assert::AreEqual("Value3 = (Value1 - 98) * Value2 == 100.0"s, copy->GetExpression());
			string newExp = "Value1 = 1 + 2 + 3 + Value1";
			copy->SetExpression(newExp);
			copy->Update(state);
			Assert::AreEqual(106, copy->Find("Value1")->AsInt());
			delete copy;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionExpressionTest::sStartMemState;
}