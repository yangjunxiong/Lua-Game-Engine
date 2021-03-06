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
	&SyntaxAnalyzer::InClassState,                 // End of class
	&SyntaxAnalyzer::InPropertyMarkState,
	&SyntaxAnalyzer::BeforePropertyState,
	&SyntaxAnalyzer::AfterPropertyState,           // End of property
	&SyntaxAnalyzer::InFunctionMarkState,
	&SyntaxAnalyzer::BeforeFunctionState,
	&SyntaxAnalyzer::InFunctionSignatureState,     // End of function
	&SyntaxAnalyzer::InConstructorMarkState,
	&SyntaxAnalyzer::BeforeConstructorState,
	&SyntaxAnalyzer::InFunctionSignatureState,     // End of contructor
	&SyntaxAnalyzer::InEnumMarkState,
	&SyntaxAnalyzer::BeforeEnumState,
	&SyntaxAnalyzer::InEnumState                   // End of enum
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

void SyntaxAnalyzer::Item::AddFlag(int& currentFlag, ItemFlag flag)
{
	currentFlag |= 1 << static_cast<int>(flag);
}

void SyntaxAnalyzer::Item::ClearFlag(int& currentFlag, ItemFlag flag)
{
	currentFlag &= 0 << static_cast<int>(flag);
}

bool SyntaxAnalyzer::Item::HasFlag(int currentFlag, ItemFlag flag)
{
	int mask = 0x00000000 | 1 << static_cast<int>(flag);
	return (currentFlag & mask) > 0;
}

void SyntaxAnalyzer::Run(const std::vector<HeaderTokenizer::Token>& tokens, std::vector<Item>& output)
{
	Reset();

	mInput = &tokens;
	mOutput = &output;

	for (size_t i = 0; i < tokens.size(); ++i)
	{
		auto func = sStateFunctions[static_cast<size_t>(mState)];
		assert(func != nullptr);
		(this->*func)(i);
	}
	if (mOutput->size() != 0 && mOutput->back().mType != ItemType::EndClass)
	{
		std::string str;
		str.reserve(1024);
		for (const auto& item : *mOutput)
		{
			char msg[1024];
			sprintf_s(msg, "Last class %s, item type %d, item name %s", mClassName.c_str(), (int)item.mType, item.mToken.String.c_str());
			str.append(msg);
		}
		throw std::runtime_error(str.c_str());
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

void SyntaxAnalyzer::ResetParseVariable()
{
	mConst = false;
	mStatic = false;
	mType.clear();
	mAfterMark = false;
	mExpectType = true;
	mExpectTypename = true;

	mExpectEnumKeyword = true;
	mExpectClassKeyword = true;
	mExpectEnumName = true;
}

bool SyntaxAnalyzer::ProcessType(std::string& typeString, const HeaderTokenizer::Token& token)
{
	assert(mExpectType);

	switch (token.Type)
	{
	case TokenType::DoubleColon:
		if (mAfterMark)
		{
			char msg[1024];
			sprintf_s(msg, "Error in class \"%s\": Following \"::\" must be a type name", mClassName.c_str());
			throw std::runtime_error(msg);
		}
		mAfterMark = true;
		typeString.append(token.String);
		return false;
	case TokenType::Name:
		if (mExpectTypename)
		{
			mExpectTypename = false;
			typeString.append(token.String);
			return false;
		}
		else if (mAfterMark)
		{
			mAfterMark = false;
			typeString.append(token.String);
			return false;
		}
		return true;
	case TokenType::Star:
	case TokenType::Ampersand:
	case TokenType::Right_AngleBracket:
		typeString.append(token.String);
		return false;
	case TokenType::Left_AngleBracket:
		mAfterMark = true;
		typeString.append(token.String);
		return false;
	case TokenType::Keyword_Const:
		typeString.append(" " + token.String + " ");
		return false;
	case TokenType::Keyword_Void:
		typeString.append(token.String);
		mExpectTypename = false;
		return false;
	}
	return false;
}

void SyntaxAnalyzer::BeforeClassMarkState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Mark_Class)
	{
		mState = State::InClassMark;
		mFlag = 0;
	}
	else if (token.Type == TokenType::Mark_Enum)
	{
		mState = State::InEnumMark;
		mFlag = 0;
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
		if (token.String == EVENT_MARK)
		{
			Item::AddFlag(mFlag, ItemFlag::EventMessage);
		}
		else if (token.String == LUA_AUTHORITY_MARK)
		{
			Item::AddFlag(mFlag, ItemFlag::NoLuaAuthority);
		}
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
		auto& item = mOutput->emplace_back(ItemType::Class, next, next.String);
		item.mFlag = mFlag;
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
		mFlag = 0;
		break;
	case TokenType::Mark_Function:
		mState = State::InFunctionMark;
		mFlag = 0;
		break;
	case TokenType::Mark_Constructor:
		mState = State::InConstructorMark;
		mFlag = 0;
		break;
	case TokenType::Mark_Enum:
	case TokenType::Mark_Class:
		char buffer[1024];
		sprintf_s(buffer, "Detect nested class or enum in class %s\n", mClassName.c_str());
		throw std::runtime_error(buffer);
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
			ResetParseVariable();
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
	case TokenType::Left_AngleBracket:
	case TokenType::Right_AngleBracket:
		if (!mExpectType || ProcessType(mType, token))
		{
			mExpectType = false;
			auto& item = mOutput->emplace_back(ItemType::Variable, token, mClassName, mParentClass, mType, mAccessLevel, !mConst, mStatic);
			item.mFlag = mFlag;
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
			ResetParseVariable();
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
	case TokenType::Keyword_Void:
	case TokenType::Keyword_Const:
	case TokenType::Left_AngleBracket:
	case TokenType::Right_AngleBracket:
		if (!mExpectType || ProcessType(mType, token))
		{
			mExpectType = false;
			auto& item = mOutput->emplace_back(ItemType::Function, token, mClassName, mParentClass, mType, mAccessLevel, !mConst, mStatic);
			item.mFlag = mFlag;
			mState = State::InFunctionSignature;
		}
		break;
	}
}

void SyntaxAnalyzer::InFunctionSignatureState(size_t index)
{
	const Token& token = mInput->at(index);
	Item& lastItem = mOutput->back();
	assert(lastItem.mType == ItemType::Constructor || lastItem.mType == ItemType::Function);

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
	case TokenType::Comma:
		if (mParenthesisLevel == mFunctionSignatureParentLevel)
		{
			lastItem.mArgumentList.emplace_back();
			mAfterMark = false;
			mExpectType = true;
			mExpectTypename = true;
		}
		break;
	case TokenType::Name:
	case TokenType::DoubleColon:
	case TokenType::Star:
	case TokenType::Ampersand:
	case TokenType::Keyword_Const:
	case TokenType::Keyword_Void:
	case TokenType::Left_AngleBracket:
	case TokenType::Right_AngleBracket:
		// If this is right after the first parenthesis, it's first argument
		if (lastItem.mArgumentList.size() == 0)
		{
			mFunctionSignatureParentLevel = mParenthesisLevel;
			lastItem.mArgumentList.emplace_back();
			mAfterMark = false;
			mExpectType = true;
			mExpectTypename = true;
		}
		if (mExpectType)
		{
			std::string& type = lastItem.mArgumentList.back();
			if (ProcessType(type, token))
			{
				mExpectType = false;
			}
		}
		break;
	}
}

void SyntaxAnalyzer::InConstructorMarkState(size_t index)
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
			mState = State::BeforeConstructor;
			ResetParseVariable();
		}
	}
	else
	{
		// TODO Additional marks
	}
}

void SyntaxAnalyzer::BeforeConstructorState(size_t index)
{
	const Token& token = mInput->at(index);
	if ((token.Type >= TokenType::Keyword_START && token.Type <= TokenType::Keyword_END) || token.Type == TokenType::Semicolon)
	{
		return;
	}

	if (token.Type != TokenType::Name || token.String != mClassName)
	{
		char msg[256];
		sprintf_s(msg, "Constructor for class \"%s\" has actual name \"%s\"", mClassName.c_str(), token.String.c_str());
		throw std::runtime_error(msg);
	}
	
	auto& item = mOutput->emplace_back(ItemType::Constructor, token, mClassName, mParentClass, mType, mAccessLevel, false, false);
	item.mFlag = mFlag;
	mState = State::InConstructorSignature;
}

void SyntaxAnalyzer::InEnumMarkState(size_t index)
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
			mState = State::BeforeEnum;
			ResetParseVariable();
		}
	}
	else
	{
		// TODO Additional marks
	}
}

void SyntaxAnalyzer::BeforeEnumState(size_t index)
{
	const Token& token = mInput->at(index);
	if (token.Type == TokenType::Keyword_Enum)
	{
		mExpectEnumKeyword = false;
	}
	else if (token.Type == TokenType::Keyword_Class)
	{
		if (mExpectEnumKeyword)
		{
			throw std::runtime_error("Put \"class\" after \"enum\"");
		}
		mExpectClassKeyword = false;
	}
	else if (token.Type == TokenType::Left_Bracket)
	{
		if (mExpectEnumKeyword || mExpectClassKeyword || mExpectEnumName)
		{
			throw std::runtime_error("Enum binding must follow \"ENUM() enum class XXX {};\"");
		}
		mState = State::InEnum;
		ResetParseVariable();
	}
	else if (token.Type == TokenType::Name)
	{
		if (mExpectEnumKeyword || mExpectClassKeyword)
		{
			throw std::runtime_error("Enum binding must follow \"ENUM() enum class XXX {};\"");
		}
		mExpectEnumName = false;
		mClassName = token.String;
		auto& item = mOutput->emplace_back(ItemType::Enum, token, mClassName, "", "", mAccessLevel, false, true);
		item.mFlag = mFlag;
	}
}

void SyntaxAnalyzer::InEnumState(size_t index)
{
	assert(mOutput->back().mType == ItemType::Enum);
	const Token& token = mInput->at(index);
	auto& enumList = mOutput->back().mEnumList;
	switch (token.Type)
	{
	case TokenType::Name:
		if (mExpectEnumName)
		{
			enumList.emplace_back(token.String);
			mExpectEnumName = false;
		}
		break;
	case TokenType::Comma:
		mExpectEnumName = true;
		break;
	case TokenType::Right_Bracket:
		mState = State::BeforeClassMark;
		ResetParseVariable();
		break;
	}
}
