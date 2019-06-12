#pragma once
#include "HeaderTokenizer.h"

namespace GameEngine::HeaderTool
{
	class SyntaxAnalyzer final
	{
	public:
		enum class ItemType : uint8_t
		{
			None,
			Class,
			EndClass,
			Variable,
			Function
		};

		enum class AccessLevel : uint8_t
		{
			Public,
			Protected,
			Private
		};

		struct Item
		{
			Item(ItemType type,
				const HeaderTokenizer::Token& token,
				const std::string& className,
				const std::string& parentClassName = std::string(),
				const std::string& memberType = std::string(),
				AccessLevel accessLevel = AccessLevel::Public,
				bool writable = true,
				bool isStatic = false);

			ItemType mType = ItemType::None;
			AccessLevel mAccessLevel = AccessLevel::Public;
			HeaderTokenizer::Token mToken;
			std::string mClassName;
			std::string mParentClassName;
			std::string mMemberType;
			bool mWritable = true;
			bool mStatic = false;
		};

		void Run(const std::vector<HeaderTokenizer::Token>& tokens, std::vector<Item>& output);

	private:
		enum class State : uint8_t
		{
			BeforeClassMark,
			InClassMark,

			BeforeClass,
			BeforeParentClass,
			InClass,

			InPropertyMark,
			BeforeProperty,
			AfterProperty,

			InFunctionMark,
			BeforeFunction,
			InFunctionSignature
		};

		using StateFunction = void (SyntaxAnalyzer::*)(size_t index);
		const static std::vector<StateFunction> sStateFunctions;
		void BeforeClassMarkState(size_t index);
		void InClassMarkState(size_t index);
		void BeforeClassState(size_t index);
		void BeforeParentClassState(size_t index);
		void InClassState(size_t index);
		void InPropertyMarkState(size_t index);
		void BeforePropertyState(size_t index);
		void AfterPropertyState(size_t index);
		void InFunctionMarkState(size_t index);
		void BeforeFunctionState(size_t index);
		void InFunctionSignatureState(size_t index);

		inline void Reset();
		inline void ResetClass();
		inline void ResetMember();

		inline bool ProcessType(const HeaderTokenizer::Token& token);

		const std::vector<HeaderTokenizer::Token>* mInput = nullptr;
		std::vector<Item>* mOutput = nullptr;

		State mState = State::BeforeClassMark;
		AccessLevel mAccessLevel = AccessLevel::Public;
		std::string mClassName;
		std::string mParentClass;
		int mBracketLevel = 0;
		int mParenthesisLevel = 0;

		// Class state
		bool mAfterColon = false;

		// Variable and function state
		bool mConst = false;
		bool mStatic = false;
		std::string mType;
		bool mAfterDoubleColon = false;
	};
}