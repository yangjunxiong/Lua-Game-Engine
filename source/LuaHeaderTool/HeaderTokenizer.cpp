#include "pch.h"
#include "HeaderTokenizer.h"
#include <list>
#include <vector>

using namespace GameEngine::HeaderTool;
using namespace std;

const std::vector<HeaderTokenizer::StateFunction> HeaderTokenizer::sStateFunction =
{
	&HeaderTokenizer::SpaceState,
	&HeaderTokenizer::IgnoreState,
	&HeaderTokenizer::WordState,
	&HeaderTokenizer::DigitState,
	&HeaderTokenizer::LeftParenState,
	&HeaderTokenizer::RightParenState,
	&HeaderTokenizer::LeftBracketState,
	&HeaderTokenizer::RightBracketState,
	&HeaderTokenizer::SemicolonState,
	&HeaderTokenizer::AssignState,
	&HeaderTokenizer::QuoteState,
	&HeaderTokenizer::LeftAngleBracketState,
	&HeaderTokenizer::RightAngleBracketState,
	&HeaderTokenizer::StarState,
	&HeaderTokenizer::AmpersandState,
	&HeaderTokenizer::ColonState,
	&HeaderTokenizer::DoubleColonState,
	&HeaderTokenizer::SlashState,
	&HeaderTokenizer::DoubleSlashState,
	&HeaderTokenizer::SlashStar,
	&HeaderTokenizer::EndStar
};

const std::map<char, HeaderTokenizer::ParseState> HeaderTokenizer::sCharacterStateMap =
{
	{ ' ', ParseState::Space },
	{ '\n', ParseState::Space },
	{ '\t', ParseState::Space },
	{ '=', ParseState::Assign },
	{ '(', ParseState::Left_Parenthesis },
	{ ')', ParseState::Right_Parenthesis },
	{ '"', ParseState::Quote },
	{ '>', ParseState::Right_AngleBracket },
	{ '<', ParseState::Left_AngleBracket },
	{ '{', ParseState::Left_Bracket },
	{ '}', ParseState::Right_Bracket },
	{ ';', ParseState::Semicolon },
	{ '*', ParseState::Star },
	{ '&', ParseState::Ampersand },
	{ ':', ParseState::Colon },
	{ '/', ParseState::Slash }
};

const std::unordered_map<std::string, HeaderTokenizer::TokenType> HeaderTokenizer::sKeywordMap =
{
	{ "class", TokenType::Keyword_Class },
	{ "struct", TokenType::Keyword_Struct },
	{ "static", TokenType::Keyword_Static },
	{ "const", TokenType::Keyword_Const },
	{ "inline", TokenType::Keyword_Inline },
	{ "public", TokenType::Keyword_Public },
	{ "protected", TokenType::Keyword_Protected },
	{ "private", TokenType::Keyword_Private },

	{ "CLASS", TokenType::Mark_Class },
	{ "PROPERTY", TokenType::Mark_Property },
	{ "FUNCTION", TokenType::Mark_Function },
};

HeaderTokenizer::Token::Token(const std::string& str, TokenType type) :
	String(str),
	Type(type)
{}

HeaderTokenizer::Token::Token(std::string&& str, TokenType type) :
	String(move(str)),
	Type(type)
{}

void HeaderTokenizer::Tokenize(const std::string& input, std::vector<Token>& output)
{
	if (input.empty())
	{
		return;
	}

	mOutput = &output;
	mParseState.push(GetParseState(input[0]));
	for (size_t i = 0; i < input.size(); ++i)
	{
		char c = input[i];
		auto func = sStateFunction[static_cast<size_t>(mParseState.top())];
		(this->*func)(c);
	}

	// Finish up the last state by inserting a space
	assert(mParseState.size() > 0);
	auto func = sStateFunction[static_cast<size_t>(mParseState.top())];
	(this->*func)(' ');
	if (!(mParseState.size() == 1 && mParseState.top() == ParseState::Space))
	{
		throw std::runtime_error("Invalid file format");
	}

	// Clear the cached output ref
	mOutput = nullptr;
}

bool HeaderTokenizer::IsDigit(const char c)
{
	return c >= '0' && c <= '9';
}

bool HeaderTokenizer::IsLetter(const char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void HeaderTokenizer::PushToken(HeaderTokenizer::TokenType type)
{
	mBuffer[mBufferLength] = 0;
	mBufferLength = 0;
	mOutput->push_back(Token(mBuffer, type));
}

HeaderTokenizer::ParseState HeaderTokenizer::GetParseState(char c)
{
	ParseState state;
	if (IsDigit(c))
	{
		state = ParseState::Digit;
	}
	else if (IsLetter(c))
	{
		state = ParseState::Word;
	}
	else
	{
		auto it = sCharacterStateMap.find(c);
		if (it != sCharacterStateMap.end())
		{
			state = it->second;
		}
		else
		{
			state = ParseState::Ignore;
		}
	}

	return state;
}

void HeaderTokenizer::NewToken(char c)
{
	mParseState.push(GetParseState(c));
	mBufferLength = 0;
	if (mParseState.top() != ParseState::Quote)
	{
		mBuffer[mBufferLength++] = c;
	}
}

#pragma region TokenizeStateMachine
void HeaderTokenizer::SpaceState(char c)
{
	mParseState.pop();
	NewToken(c);
}

void HeaderTokenizer::IgnoreState(char c)
{
	mParseState.pop();
	NewToken(c);
}

void HeaderTokenizer::WordState(char c)
{
	if (IsLetter(c) || IsDigit(c))
	{
		mBuffer[mBufferLength++] = c;
	}
	else
	{
		mParseState.pop();
		mBuffer[mBufferLength] = 0;
		auto it = sKeywordMap.find(std::string(mBuffer));
		if (it != sKeywordMap.end())
		{
			PushToken(it->second);
		}
		else
		{
			PushToken(TokenType::Name);
		}
		NewToken(c);
	}
}

void HeaderTokenizer::DigitState(char c)
{
	if (IsDigit(c) || c == '.')
	{
		mBuffer[mBufferLength++] = c;
	}
	else if (c != 'f' && c != 'L')
	{
		PushToken(TokenType::Literal_Number);
		mParseState.pop();
		NewToken(c);
	}
}

void HeaderTokenizer::LeftParenState(char c)
{
	// Push token but don't pop stack
	PushToken(TokenType::Left_Parenthesis);
	NewToken(c);
}

void HeaderTokenizer::RightParenState(char c)
{
	PushToken(TokenType::Right_Parenthesis);
	mParseState.pop();  // Pop ')'
	if (mParseState.size() > 0 && mParseState.top() == ParseState::Left_Parenthesis)
	{
		mParseState.pop();  // Pop '('
		NewToken(c);
	}
	else
	{
		throw std::runtime_error("Parenthesis mismatch");
	}
}

void HeaderTokenizer::LeftBracketState(char c)
{
	PushToken(TokenType::Left_Bracket);
	NewToken(c);
}

void HeaderTokenizer::RightBracketState(char c)
{
	PushToken(TokenType::Right_Bracket);
	mParseState.pop();  // Pop '}'
	if (mParseState.size() > 0 && mParseState.top() == ParseState::Left_Bracket)
	{
		mParseState.pop();  // Pop '{'
		NewToken(c);
	}
	else
	{
		throw std::runtime_error("Bracket mismatch");
	}
}

void HeaderTokenizer::SemicolonState(char c)
{
	PushToken(TokenType::Semicolon);
	mParseState.pop();
	NewToken(c);
}

void HeaderTokenizer::AssignState(char c)
{
	PushToken(TokenType::Assign);
	mParseState.pop();
	NewToken(c);
}

void HeaderTokenizer::QuoteState(char c)
{
	if (c == '"' && mBuffer[mBufferLength - 1] != '\\')
	{
		PushToken(TokenType::Literal_String);
		mParseState.pop();                     // Pop left '"'
		mParseState.push(ParseState::Ignore);  // Ignore the closing quotation mark
	}
	else
	{
		if (mBuffer[mBufferLength - 1] == '\\' && (c == '"' || c == '\\'))
		{
			mBuffer[mBufferLength - 1] = c;
		}
		else
		{
			mBuffer[mBufferLength++] = c;
		}
	}
}

void HeaderTokenizer::LeftAngleBracketState(char c)
{
	PushToken(TokenType::Left_AngleBracket);
	mParseState.pop();
	NewToken(c);
}

void HeaderTokenizer::RightAngleBracketState(char c)
{
	PushToken(TokenType::Right_AngleBracket);
	mParseState.pop();  // Pop '>'
	NewToken(c);
}

void HeaderTokenizer::StarState(char c)
{
	PushToken(TokenType::Star);
	mParseState.pop();
	NewToken(c);
}

void HeaderTokenizer::AmpersandState(char c)
{
	PushToken(TokenType::Ampersand);
	mParseState.pop();
	NewToken(c);
}

void HeaderTokenizer::ColonState(char c)
{
	mParseState.pop();
	if (c == ':')
	{
		mParseState.push(ParseState::DoubleColon);
		mBuffer[mBufferLength++] = c;
	}
	else
	{
		PushToken(TokenType::Colon);
		NewToken(c);
	}
}

void HeaderTokenizer::DoubleColonState(char c)
{
	PushToken(TokenType::DoubleColon);
	mParseState.pop();
	NewToken(c);
}

void HeaderTokenizer::SlashState(char c)
{
	mParseState.pop();
	if (c == '/')
	{
		mParseState.push(ParseState::DoubleSlash);
	}
	else if (c == '*')
	{
		mParseState.push(ParseState::SlashStar);
	}
	else
	{
		PushToken(TokenType::Slash);
		NewToken(c);
	}
}

void HeaderTokenizer::DoubleSlashState(char c)
{
	if (c == '\n')
	{
		mParseState.pop();
		NewToken(c);
	}
}

void HeaderTokenizer::SlashStar(char c)
{
	if (c == '*')
	{
		mParseState.pop();
		mParseState.push(ParseState::EndStar);
	}
}

void HeaderTokenizer::EndStar(char c)
{
	if (c == '/')
	{
		mParseState.pop();
		NewToken(' ');
	}
	else if (c != '*')
	{
		mParseState.pop();
		mParseState.push(ParseState::SlashStar);
	}
}
#pragma endregion
