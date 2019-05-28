#include "pch.h"
#include "Tokenizer.h"
#include "SList.h"
#include "vector.h"

using namespace GameEngine;
using namespace std;

Vector<Tokenizer::StateFunction> Tokenizer::sStateFunction =
{
	&Tokenizer::EmptyState,
	&Tokenizer::DigitState,
	&Tokenizer::DotState,
	&Tokenizer::FloatDigitState,
	&Tokenizer::VariableState,
	&Tokenizer::PlusState,
	&Tokenizer::MinusState,
	&Tokenizer::MultiplyState,
	&Tokenizer::DivideState,
	&Tokenizer::AssignState,
	&Tokenizer::LeftParenState,
	&Tokenizer::RightParenState,
	&Tokenizer::QuoteState,
	&Tokenizer::ModState,
	&Tokenizer::NotState,
	&Tokenizer::GreaterState,
	&Tokenizer::LessState,
	&Tokenizer::BoolAndState,
	&Tokenizer::BoolOrState
};

HashMap<char, Tokenizer::ParseState> Tokenizer::sCharacterStateMap =
{
	{ '+', ParseState::Plus },
	{ '-', ParseState::Minus },
	{ '*', ParseState::Multiply },
	{ '/', ParseState::Divide },
	{ '=', ParseState::Assign },
	{ '(', ParseState::LeftParen },
	{ ')', ParseState::RightParen },
	{ '"', ParseState::Quote },
	{ '%', ParseState::Mod },
	{ '!', ParseState::Not },
	{ '>', ParseState::Greater },
	{ '<', ParseState::Less },
	{ '.', ParseState::Variable },
	{ '&', ParseState::BoolAnd },
	{ '|', ParseState::BoolOr }
};

Tokenizer::Token::Token(const std::string& str, TokenType type) :
	String(str),
	Type(type)
{}

bool Tokenizer::Token::IsOperator() const
{
	return Type != TokenType::Integer && Type != TokenType::Float && Type != TokenType::Variable && Type != TokenType::StringLiteral
		&& Type != TokenType::LeftParen && Type != TokenType::RightParen;
}

bool Tokenizer::Token::IsUnaryOperator() const
{
	return Type == TokenType::Negate || Type == TokenType::Not;
}

void Tokenizer::Tokenize(const std::string& input, Vector<Token>& output)
{
	ParseState state = ParseState::Empty;
	char buffer[256];
	size_t charIndex = 0;

	for (size_t i = 0; i < input.size(); ++i)
	{
		char c = input[i];
		auto func = sStateFunction[size_t(state)];
		(*func)(state, buffer, charIndex, c, output);
	}

	// Deal with the last token
	if (state == ParseState::Digit)
	{
		PushToken(buffer, TokenType::Integer, charIndex, output);
	}
	else if (state == ParseState::FloatDigit)
	{
		PushToken(buffer, TokenType::Float, charIndex, output);
	}
	else if (state == ParseState::Variable)
	{
		PushToken(buffer, TokenType::Variable, charIndex, output);
	}
	else if (state == ParseState::RightParen)
	{
		PushToken(buffer, TokenType::RightParen, charIndex, output);
	}
}

void Tokenizer::ConvertToRPN(const Vector<Token>& input, Vector<Token>& output)
{
	// Shunting yard algorithm to transfer to RPN
	SList<const Token*> operatorStack;
	Token* assignee = nullptr;
	for (auto& token : input)
	{
		// Parenthesis case
		if (token.Type == TokenType::LeftParen)
		{
			operatorStack.PushFront(&token);
		}
		else if (token.Type == TokenType::RightParen)
		{
			while (operatorStack.Front()->Type != TokenType::LeftParen)
			{
				output.PushBack(*operatorStack.Front());
				operatorStack.PopFront();
				if (operatorStack.IsEmpty())
				{
					throw exception("() mismatch");
				}
			}
			operatorStack.PopFront();  // Pop the ")"
		}

		// Operator case
		else if (token.IsOperator())
		{
			if (token.Type == TokenType::Assign)
			{
				assignee = new Token(output.Back());
				output.PopBack();
				operatorStack.PushBack(&token);  // Always put the assign operator at the end
			}
			else
			{
				while (!operatorStack.IsEmpty() && operatorStack.Front()->Type != TokenType::LeftParen && ComparePrecedence(token, *operatorStack.Front()) <= 0)
				{
					output.PushBack(*operatorStack.Front());
					operatorStack.PopFront();
				}
				operatorStack.PushFront(&token);
			}
		}

		// Normal symbol case
		else
		{
			output.PushBack(token);
		}
	}

	// Pop rest operators and handle assignee
	while (!operatorStack.IsEmpty())
	{
		// Put assignee first then the last operator should be "="
		if (assignee != nullptr && operatorStack.Size() == 1)
		{
			assert(operatorStack.Front()->Type == TokenType::Assign);
			output.PushBack(*assignee);
			delete assignee;
			assignee = nullptr;
		}

		output.PushBack(*operatorStack.Front());
		operatorStack.PopFront();
	}
}

bool Tokenizer::IsDigit(const char& c)
{
	return c >= '0' && c <= '9';
}

bool Tokenizer::IsLetter(const char& c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void Tokenizer::PushToken(char* buffer, Tokenizer::TokenType type, size_t& charIndex, Vector<Token>& output)
{
	buffer[charIndex] = 0;
	charIndex = 0;
	output.PushBack(Token(buffer, type));
}

Tokenizer::ParseState Tokenizer::GetParseState(char c)
{
	ParseState state;
	if (IsDigit(c))
	{
		state = ParseState::Digit;
	}
	else if (IsLetter(c))
	{
		state = ParseState::Variable;
	}
	else
	{
		auto it = sCharacterStateMap.Find(c);
		if (it != sCharacterStateMap.end())
		{
			state = it->second;
		}
		else
		{
			throw exception("Unknown symbol");
		}
	}

	return state;
}

int Tokenizer::ComparePrecedence(const Token& token1, const Token& token2)
{
	int pre1 = GetPrecedence(token1);
	int pre2 = GetPrecedence(token2);
	if (pre1 == pre2)
	{
		return 0;
	}
	else
	{
		return pre1 < pre2 ? -1 : 1;
	}
}

int Tokenizer::GetPrecedence(const Token& token)
{
	if (token.Type == TokenType::Assign)
	{
		return 0;
	}
	else if (token.Type == TokenType::BoolAnd)
	{
		return 1;
	}
	else if (token.Type == TokenType::BoolOr)
	{
		return 2;
	}
	else if (token.Type == TokenType::Equal || token.Type == TokenType::NotEqual
			 || token.Type == TokenType::GreaterThan || token.Type == TokenType::GreaterEqual
			 || token.Type == TokenType::LessThan || token.Type == TokenType::LessEqual)
	{
		return 3;
	}
	
	else if (token.Type == TokenType::Add || token.Type == TokenType::Minus)
	{
		return 4;
	}
	else if (token.Type == TokenType::Multiply || token.Type == TokenType::Divide || token.Type == TokenType::Mod)
	{
		return 5;
	}
	else if (token.Type == TokenType::Negate || token.Type == TokenType::Not)  // Unary
	{
		return 6;
	}

	return -1;
}

#pragma region TokenizeStateMachine
void Tokenizer::EmptyState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	state = ParseState::Empty;
	if (newChar != ' ')
	{
		buffer[charIndex++] = newChar;
		state = GetParseState(newChar);

		// For variable path, '/' might be start of path like "/path1/path2" instead of divide
		if (state == ParseState::Divide)
		{
			if (output.Size() == 0 || output.Back().IsOperator() || output.Back().Type == TokenType::LeftParen)
			{
				state = ParseState::Variable;
			}
		}
	}
}

void Tokenizer::DigitState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (IsDigit(newChar))
	{
		buffer[charIndex++] = newChar;
	}
	else if (newChar == '.')
	{
		buffer[charIndex++] = newChar;
		state = ParseState::Dot;
	}
	else
	{
		PushToken(buffer, TokenType::Integer, charIndex, output);
		EmptyState(state, buffer, charIndex, newChar, output);
	}
}

void Tokenizer::DotState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& /*output*/)
{
	if (IsDigit(newChar))
	{
		buffer[charIndex++] = newChar;
		state = ParseState::FloatDigit;
	}
	else
	{
		throw exception("Expect digit after '.'");
	}
}

void Tokenizer::FloatDigitState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (IsDigit(newChar))
	{
		buffer[charIndex++] = newChar;
	}
	else
	{
		PushToken(buffer, TokenType::Float, charIndex, output);
		EmptyState(state, buffer, charIndex, newChar, output);
	}
}

void Tokenizer::VariableState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (IsLetter(newChar) || IsDigit(newChar) || newChar == '.' || newChar == '/' || newChar == '[' || newChar == ']')
	{
		buffer[charIndex++] = newChar;
	}
	else
	{
		PushToken(buffer, TokenType::Variable, charIndex, output);
		EmptyState(state, buffer, charIndex, newChar, output);
	}
}

void Tokenizer::PlusState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	PushToken(buffer, TokenType::Add, charIndex, output);
	EmptyState(state, buffer, charIndex, newChar, output);
}

void Tokenizer::MinusState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (IsDigit(newChar) || IsLetter(newChar))
	{
		if (output.Size() == 0 || output.Back().IsOperator() || output.Back().Type == TokenType::LeftParen)
		{
			PushToken(buffer, TokenType::Negate, charIndex, output);
		}
		else
		{
			PushToken(buffer, TokenType::Minus, charIndex, output);
		}
	}
	else
	{
		PushToken(buffer, TokenType::Minus, charIndex, output);
	}
	EmptyState(state, buffer, charIndex, newChar, output);
}

void Tokenizer::MultiplyState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	PushToken(buffer, TokenType::Multiply, charIndex, output);
	EmptyState(state, buffer, charIndex, newChar, output);
}

void Tokenizer::DivideState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	PushToken(buffer, TokenType::Divide, charIndex, output);
	EmptyState(state, buffer, charIndex, newChar, output);
}

void Tokenizer::AssignState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (newChar == '=')
	{
		buffer[charIndex++] = newChar;
		PushToken(buffer, TokenType::Equal, charIndex, output);
		state = ParseState::Empty;
	}
	else
	{
		PushToken(buffer, TokenType::Assign, charIndex, output);
		EmptyState(state, buffer, charIndex, newChar, output);
	}
}

void Tokenizer::LeftParenState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	PushToken(buffer, TokenType::LeftParen, charIndex, output);
	EmptyState(state, buffer, charIndex, newChar, output);
}

void Tokenizer::RightParenState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	PushToken(buffer, TokenType::RightParen, charIndex, output);
	EmptyState(state, buffer, charIndex, newChar, output);
}

void Tokenizer::QuoteState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (newChar != '"')
	{
		buffer[charIndex++] = newChar;
	}
	else
	{
		for (size_t i = 0; i < charIndex - 1; ++i)  // Remove the frist "
		{
			buffer[i] = buffer[i + 1];
		}
		--charIndex;
		PushToken(buffer, TokenType::StringLiteral, charIndex, output);
		state = ParseState::Empty;
	}
}

void Tokenizer::ModState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	PushToken(buffer, TokenType::Mod, charIndex, output);
	EmptyState(state, buffer, charIndex, newChar, output);
}

void Tokenizer::NotState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (newChar == '=')
	{
		buffer[charIndex++] = newChar;
		PushToken(buffer, TokenType::NotEqual, charIndex, output);
		state = ParseState::Empty;
	}
	else
	{
		PushToken(buffer, TokenType::Not, charIndex, output);
		EmptyState(state, buffer, charIndex, newChar, output);
	}
}

void Tokenizer::GreaterState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (newChar == '=')
	{
		buffer[charIndex++] = newChar;
		PushToken(buffer, TokenType::GreaterEqual, charIndex, output);
		state = ParseState::Empty;
	}
	else
	{
		PushToken(buffer, TokenType::GreaterThan, charIndex, output);
		EmptyState(state, buffer, charIndex, newChar, output);
	}
}

void Tokenizer::LessState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (newChar == '=')
	{
		buffer[charIndex++] = newChar;
		PushToken(buffer, TokenType::LessEqual, charIndex, output);
		state = ParseState::Empty;
	}
	else
	{
		PushToken(buffer, TokenType::LessThan, charIndex, output);
		EmptyState(state, buffer, charIndex, newChar, output);
	}
}

void Tokenizer::BoolAndState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (newChar == '&')
	{
		buffer[charIndex++] = newChar;
		PushToken(buffer, TokenType::BoolAnd, charIndex, output);
		state = ParseState::Empty;
	}
	else
	{
		throw exception("Expect second &");
	}
}

void Tokenizer::BoolOrState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output)
{
	if (newChar == '|')
	{
		buffer[charIndex++] = newChar;
		PushToken(buffer, TokenType::BoolOr, charIndex, output);
		state = ParseState::Empty;
	}
	else
	{
		throw exception("Expect second |");
	}
}
#pragma endregion
