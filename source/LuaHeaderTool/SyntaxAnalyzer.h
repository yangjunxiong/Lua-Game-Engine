#pragma once
#include "HeaderTokenizer.h"

#define EVENT_MARK "EventMessage"
#define LUA_AUTHORITY_MARK "NoLuaAuthority"

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
			Function,
			Constructor,
			Enum
		};

		enum class AccessLevel : uint8_t
		{
			Public,
			Protected,
			Private
		};

		enum class ItemFlag : int
		{
			EventMessage = 0,
			NoLuaAuthority = 1
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

			static void AddFlag(int& currentFlag, ItemFlag flag);
			static void ClearFlag(int& currentFlag, ItemFlag flag);
			static bool HasFlag(int currentFlag, ItemFlag flag);

			ItemType mType = ItemType::None;
			AccessLevel mAccessLevel = AccessLevel::Public;
			HeaderTokenizer::Token mToken;
			std::string mClassName;
			std::string mParentClassName;
			std::string mMemberType;
			bool mWritable = true;
			bool mStatic = false;
			std::vector<std::string> mArgumentList;
			std::vector<std::string> mEnumList;
			int mFlag = 0;
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
			InFunctionSignature,

			InConstructorMark,
			BeforeConstructor,
			InConstructorSignature,

			InEnumMark,
			BeforeEnum,
			InEnum,
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
		void InConstructorMarkState(size_t index);
		void BeforeConstructorState(size_t index);
		void InEnumMarkState(size_t index);
		void BeforeEnumState(size_t index);
		void InEnumState(size_t index);

		inline void Reset();
		inline void ResetClass();
		inline void ResetParseVariable();

		inline bool ProcessType(std::string& typeString, const HeaderTokenizer::Token& token);

		const std::vector<HeaderTokenizer::Token>* mInput = nullptr;
		std::vector<Item>* mOutput = nullptr;

		State mState = State::BeforeClassMark;
		AccessLevel mAccessLevel = AccessLevel::Public;
		std::string mClassName;
		std::string mParentClass;
		int mBracketLevel = 0;
		int mParenthesisLevel = 0;

		/// <summary>
		/// Special flag attribute to specify in the macro for property, function, constructor or enum.
		/// The final flag for one macro will be bitwise add result for all attributes in the macro
		/// </summary>
		/// <see cref="ItemFlag"/>
		int mFlag = 0;

		// Class state
		bool mAfterColon = false;

		// Variable and function state
		bool mConst = false;
		bool mStatic = false;
		std::string mType;
		bool mAfterMark = false;
		bool mExpectType = false;
		bool mExpectTypename = false;
		int mFunctionSignatureParentLevel = 0;

		// Enum state
		bool mExpectEnumKeyword = true;
		bool mExpectClassKeyword = true;
		bool mExpectEnumName = true;
	};
}