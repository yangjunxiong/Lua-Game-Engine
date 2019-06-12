#include "pch.h"
#include "SyntaxAnalyzer.h"

using namespace GameEngine::HeaderTool;
using Token = HeaderTokenizer::Token;
using TokenType = HeaderTokenizer::TokenType;

const std::vector<SyntaxAnalyzer::StateFunction> SyntaxAnalyzer::sStateFunctions =
{
	&SyntaxAnalyzer::BeforeClassMarkState,
	&SyntaxAnalyzer::InClassMarkState,
	&SyntaxAnalyzer::BeforeClassState,
	&SyntaxAnalyzer::BeforeParentClassState,
	&SyntaxAnalyzer::InClassState,
	&SyntaxAnalyzer::InPropertyMarkState,
	&SyntaxAnalyzer::BeforePropertyState,
	&SyntaxAnalyzer::AfterPropertyState,
	&SyntaxAnalyzer::InFunctionMarkState,
	&SyntaxAnalyzer::BeforeFunctionState,
	&SyntaxAnalyzer::InFunctionSignatureState
};

SyntaxAnalyzer::Item::Item(ItemType type,
	const HeaderTokenizer::Token& token,
	const std::string& className,
	const std::string& parentClassName,
	const std::string& memberType,
	AccessLevel accessLevel,
	bool writable,
	bool isStatic) :
	mType(type),
	mToken(token),
	mClassName(className),
	mMemberType(memberType),
	mParentClassName(parentClassName),
	mAccessLevel(accessLevel),
	mWritable(writable),
	mStatic(isStatic)
{}

void SyntaxAnalyzer::Run(const std::vector<HeaderTokenizer::Token>& tokens, std::vector<Item>& output)
{
	Reset();

	mInput = &tokens;
	mOutput = &output;

	for (int i = 0; i < tokens.size(); ++i)
	{
		auto func = sStateFunctions[static_cast<size_t>(mState)];
		assert(func != nullptr);
		(this->*func)(i);
	}
}

void SyntaxAnalyzer::Reset()
{
	mState = State::BeforeClassMark;
	mAccessLevel = AccessLevel::Public;
	mClassName.clear();
	mParentClass.clear();
	mBracketLevel = 0;
	mParenthesisLevel = 0;
}

void SyntaxAnalyzer::ResetClass()
{
	mAfterColon = false;
}

void SyntaxAnalyzer::ResetMember()
{
	mConst = false;
	mStatic = false;
	mType.clear();
	mAfterDoubleColon = false;
}

bool SyntaxAnalyzer::ProcessType(const HeaderTokenizer::Token& token)
{
	if (mType.empty())
	{
		mType = token.String;
		return false;
	}
	else
	{
		switch (token.Type)
		{
		case TokenType::DoubleColon:
			if (mAfterDoubleColon)
			{
				throw std::runtime_error("Following \"::\" must be a type name");
			}
			mAfterDoubleColon = true;
			mType.append(token.String);
			return false;
		case TokenType::Name:
			if (mAfterDoubleColon)
			{
				mAfterDoubleColon = false;
				mType.append(token.String);
				return false;
			}
			return true;
		case TokenType::Star:
		case TokenType::Ampersand:
			mType.append(token.String);
			return false;
		}
		return false;
	}
}

void SyntaxAnalyzer::BeforeClassMarkState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Mark_Class)
	{
		mState = State::InClassMark;
	}
}

void SyntaxAnalyzer::InClassMarkState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Left_Parenthesis)
	{
		++mParenthesisLevel;
	}
	else if (token.Type == TokenType::Right_Parenthesis)
	{
		--mParenthesisLevel;
		if (mParenthesisLevel == 0)
		{
			mState = State::BeforeClass;
			ResetClass();
		}
	}
	else
	{
		// TODO Additional marks
	}
}

void SyntaxAnalyzer::BeforeClassState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Keyword_Class || token.Type == TokenType::Keyword_Struct)
	{
		if (mParenthesisLevel != 0 || mBracketLevel != 0)
		{
			throw std::runtime_error("Closing symbol mismatch");
		}
		if (index + 1 >= mInput->size())
		{
			throw std::runtime_error("Following class keyword should be class name");
		}
		const Token& next = mInput->at(index + 1);
		if (next.Type != TokenType::Name)
		{
			throw std::runtime_error("Following class keyword should be class name");
		}

		mClassName = next.String;
		mOutput->emplace_back(ItemType::Class, next, next.String);
		mState = State::BeforeParentClass;
		mAccessLevel = token.Type == TokenType::Keyword_Class ? AccessLevel::Private : AccessLevel::Public;
	}
}

void SyntaxAnalyzer::BeforeParentClassState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Colon)
	{
		mAfterColon = true;
	}
	else if (token.Type == TokenType::Name && mAfterColon)
	{
		if (mOutput->size() < 1 || mOutput->back().mType != ItemType::Class)
		{
			throw std::runtime_error("Parent class must have child class");
		}
		mOutput->back().mParentClassName = token.String;
		mState = State::InClass;
	}
	else if (token.Type == TokenType::Left_Bracket)
	{
		mState = State::InClass;
		++mBracketLevel;
	}
}

void SyntaxAnalyzer::InClassState(size_t index)
{
	const Token& token = mInput->at(index);
	switch (token.Type)
	{
	case TokenType::Left_Bracket:
		++mBracketLevel;
		break;
	case TokenType::Right_Bracket:
		--mBracketLevel;
		if (mBracketLevel == 0)
		{
			mOutput->emplace_back(ItemType::EndClass, token, mClassName);
			mState = State::BeforeClassMark;
		}
		break;
	case TokenType::Mark_Property:
		mState = State::InPropertyMark;
		break;
	case TokenType::Mark_Function:
		mState = State::InFunctionMark;
		break;
	case TokenType::Keyword_Public:
		mAccessLevel = AccessLevel::Public;
		break;
	case TokenType::Keyword_Protected:
		mAccessLevel = AccessLevel::Protected;
		break;
	case TokenType::Keyword_Private:
		mAccessLevel = AccessLevel::Private;
		break;
	}
}

void SyntaxAnalyzer::InPropertyMarkState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Left_Parenthesis)
	{
		++mParenthesisLevel;
	}
	else if (token.Type == TokenType::Right_Parenthesis)
	{
		--mParenthesisLevel;
		if (mParenthesisLevel == 0)
		{
			mState = State::BeforeProperty;
			ResetMember();
		}
	}
	else
	{
		// TODO Additional marks
	}
}

void SyntaxAnalyzer::BeforePropertyState(size_t index)
{
	const Token& token = mInput->at(index);
	switch (token.Type)
	{
	case TokenType::Keyword_Const:
		mConst = true;
		break;
	case TokenType::Keyword_Static:
		mStatic = true;
		break;
	case TokenType::Name:
	case TokenType::DoubleColon:
	case TokenType::Star:
	case TokenType::Ampersand:
		if (ProcessType(token))
		{
			mOutput->emplace_back(ItemType::Variable, token, mClassName, mParentClass, mType, mAccessLevel, !mConst, mStatic);
			mState = State::AfterProperty;
		}
		break;
	}
}

void SyntaxAnalyzer::AfterPropertyState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Semicolon)
	{
		mState = State::InClass;
	}
}

void SyntaxAnalyzer::InFunctionMarkState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Left_Parenthesis)
	{
		++mParenthesisLevel;
	}
	else if (token.Type == TokenType::Right_Parenthesis)
	{
		--mParenthesisLevel;
		if (mParenthesisLevel == 0)
		{
			mState = State::BeforeFunction;
			ResetMember();
		}
	}
	else
	{
		// TODO Additional marks
	}
}

void SyntaxAnalyzer::BeforeFunctionState(size_t index)
{
	const Token& token = mInput->at(index);
	switch (token.Type)
	{
	case TokenType::Keyword_Static:
		mStatic = true;
		break;
	case TokenType::Name:
	case TokenType::DoubleColon:
	case TokenType::Star:
	case TokenType::Ampersand:
		if (ProcessType(token))
		{
			mOutput->emplace_back(ItemType::Function, token, mClassName, mParentClass, mType, mAccessLevel, !mConst, mStatic);
			mState = State::InFunctionSignature;
		}
		break;
	}
}

void SyntaxAnalyzer::InFunctionSignatureState(size_t index)
{
	const Token& token = mInput->at(index);
	switch (token.Type)
	{
	case TokenType::Left_Parenthesis:
		++mParenthesisLevel;
		break;
	case TokenType::Right_Parenthesis:
		--mParenthesisLevel;
		if (mParenthesisLevel == 0)
		{
			mState = State::InClass;
		}
		break;
	}
}