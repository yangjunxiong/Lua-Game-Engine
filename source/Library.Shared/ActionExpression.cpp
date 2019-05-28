#include "pch.h"
#include "ActionExpression.h"
#include "Datum.h"
#include "SList.h"
#include "Stack.h"

using namespace GameEngine;
using namespace std;
using Token = Tokenizer::Token;
using TokenType = Tokenizer::TokenType;
RTTI_DEFINITIONS(ActionExpression);

ActionExpression::ActionExpression() : Action(ActionExpression::TypeIdClass())
{}

void ActionExpression::Update(WorldState& state)
{
	Action::Update(state);
	Evaluate();

	Stack<Operand> stack;
	for (size_t i = 0; i < mTokens.Size(); ++i)
	{
		Token& token = mTokens[i];
		if (token.IsOperator())  // Do operations upon finding operator
		{
			if (token.IsUnaryOperator())
			{
				Operand operand = stack.Peak();
				stack.Pop();
				FillOperand(operand);

				if (token.Type == TokenType::Negate)
				{
					operand.mDatum = -operand.mDatum;
				}
				else if (token.Type == TokenType::Not)
				{
					operand.mDatum = !operand.mDatum;
				}
				stack.Push(Operand(move(operand.mDatum)));
			}
			else
			{
				Operand operand2 = stack.Peak();
				stack.Pop();
				assert(!stack.IsEmpty());
				Operand operand1 = stack.Peak();
				stack.Pop();

				if (token.Type == TokenType::Assign)
				{
					assert(!operand2.mString.empty());
					auto[datum, scope] = Search(operand2.mString);
					assert(datum != nullptr && datum->Type() == operand1.mDatum.Type());
					datum->CopyValue(operand1.mDatum);
				}
				else
				{
					FillOperand(operand1);
					FillOperand(operand2);
					Datum intermidiate;
					switch (token.Type)
					{
					case TokenType::Add:
						intermidiate = operand1.mDatum + operand2.mDatum;
						break;
					case TokenType::Minus:
						intermidiate = operand1.mDatum - operand2.mDatum;
						break;
					case TokenType::Multiply:
						intermidiate = operand1.mDatum * operand2.mDatum;
						break;
					case TokenType::Divide:
						intermidiate = operand1.mDatum / operand2.mDatum;
						break;
					case TokenType::Mod:
						intermidiate = operand1.mDatum % operand2.mDatum;
						break;
					case TokenType::Equal:
						intermidiate = operand1.mDatum == operand2.mDatum ? 1 : 0;
						break;
					case TokenType::NotEqual:
						intermidiate = operand1.mDatum != operand2.mDatum ? 1 : 0;
						break;
					case TokenType::GreaterThan:
						intermidiate = operand1.mDatum > operand2.mDatum ? 1 : 0;
						break;
					case TokenType::GreaterEqual:
						intermidiate = operand1.mDatum >= operand2.mDatum ? 1 : 0;
						break;
					case TokenType::LessThan:
						intermidiate = operand1.mDatum < operand2.mDatum ? 1 : 0;
						break;
					case TokenType::LessEqual:
						intermidiate = operand1.mDatum <= operand2.mDatum ? 1 : 0;
						break;
					case TokenType::BoolAnd:
						intermidiate = operand1.mDatum && operand2.mDatum ? 1 : 0;
						break;
					case TokenType::BoolOr:
						intermidiate = operand1.mDatum || operand2.mDatum ? 1 : 0;
						break;
					}
					stack.Push(Operand(intermidiate));
				}
			}
		}
		else  // Push normal symbols to the stack
		{
			if (token.Type == TokenType::Variable)
			{
				stack.Push(Operand(token.String));
			}
			else if (token.Type == TokenType::Integer)
			{
				stack.Push(Operand(Datum(stoi(token.String))));
			}
			else if (token.Type == TokenType::Float)
			{
				stack.Push(Operand(Datum(stof(token.String))));
			}
			else if (token.Type == TokenType::StringLiteral)
			{
				stack.Push(Operand(Datum(token.String)));
			}
		}
	}

	assert(stack.Size() <= 1);
}

gsl::owner<ActionExpression*> ActionExpression::Clone() const
{
	return new ActionExpression(*this);
}

void ActionExpression::SetExpression(const std::string& exp)
{
	mExpression = exp;
	mDirty = true;
}

void ActionExpression::SetExpression(std::string&& exp)
{
	mExpression = move(exp);
	mDirty = true;
}

const std::string& ActionExpression::GetExpression() const
{
	return mExpression;
}

const Vector<Attributed::Signature> ActionExpression::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Expression", Datum::DatumType::String, 1, offsetof(ActionExpression, mExpression))
	});
}

void ActionExpression::Evaluate()
{
	if (mDirty)
	{
		mDirty = false;
		Vector<Token> tokens;
		Tokenizer::Tokenize(mExpression, tokens);
		mTokens.Clear();
		Tokenizer::ConvertToRPN(tokens, mTokens);
	}
}

void ActionExpression::FillOperand(Operand& operand) const
{
	if (!operand.mString.empty())
	{
		auto[datum, scope] = Search(operand.mString);
		assert(datum != nullptr && datum->Type() != Datum::DatumType::Unknown);
		operand.mDatum = *datum;
	}
}
