#pragma once
#include <vector>
#include <unordered_map>
#include <stack>
#include <functional>

namespace GameEngine::HeaderTool
{
	/// <summary>
	/// Tokenize the expression string and convert it to RPN notation
	/// </summary>
	class HeaderTokenizer final
	{
	public:
		/// <summary>
		/// Type of token
		/// </summary>
		enum class TokenType : uint8_t
		{
			Ignore,
			Left_Parenthesis,
			Right_Parenthesis,
			Left_Bracket,
			Right_Bracket,
			Left_AngleBracket,
			Right_AngleBracket,
			Semicolon,
			Comma,
			Assign,
			Star,
			Ampersand,
			Colon,
			DoubleColon,

			Mark_Class,
			Mark_Property,
			Mark_Function,

			Keyword_Class,
			Keyword_Struct,
			Keyword_Static,
			Keyword_Const,
			Keyword_Inline,
			Keyword_Public,
			Keyword_Protected,
			Keyword_Private,
			
			Name,
			Literal_Number,
			Literal_String
		};

		/// <summary>
		/// A token parsed from the expression string
		/// </summary>
		struct Token
		{
			/// <summary>
			/// Raw string of this token
			/// </summary>
			std::string String;

			/// <summary>
			/// Type of token
			/// </summary>
			TokenType Type;

			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="str">String of this token</param>
			/// <param name="type">Token type</param>
			Token(const std::string& str, TokenType type);
			Token(std::string&& str, TokenType type);
		};

		/// <summary>
		/// Given an expression string, tokenize it and fill the output vector
		/// </summary>
		/// <param name="input">Expression string</param>
		/// <param name="output">Output vector of tokens, should be empty when passed in</param>
		void Tokenize(const std::string& input, std::vector<Token>& output);

	private:
		/// <summary>
		/// The state of parsing state machine
		/// </summary>
		enum class ParseState : uint8_t
		{
			Space,
			Ignore,
			Word,
			Digit,

			Left_Parenthesis,
			Right_Parenthesis,
			Left_Bracket,
			Right_Bracket,
			Semicolon,
			Assign,
			Quote,
			Left_AngleBracket,
			Right_AngleBracket,
			Star,
			Ampersand,
			Colon,
			DoubleColon,

			Slash,
			DoubleSlash,
			SlashStar,
			EndStar,
		};

		/// <summary>
		/// Function type to handle each state in state machine
		/// </summary>
		using StateFunction = void (HeaderTokenizer::*) (char);

		/// <summary>
		/// Current parsing state stack
		/// </summary>
		std::stack<ParseState> mParseState;

		/// <summary>
		/// The buffer array to hold current token
		/// </summary>
		char mBuffer[1024];

		/// <summary>
		/// Current index to the buffer array
		/// </summary>
		size_t mBufferLength = 0;

		/// <summary>
		/// Cached output vector, only valid within "Tokenize" call, other times will be nullptr
		/// </summary>
		std::vector<Token>* mOutput = nullptr;

		/// <summary>
		/// Check if the char is digit
		/// </summary>
		/// <param name="c">input char</param>
		/// <returns>True if the char is digit</returns>
		static inline bool IsDigit(const char c);

		/// <summary>
		/// Check if the char is alphabetical letter
		/// </summary>
		/// <param name="c">input char</param>
		/// <returns>True if the char is letter</returns>
		static inline bool IsLetter(const char c);

		/// <summary>
		/// Push a parsed token to the token vector
		/// </summary>
		/// <param name="buffer">String of token</param>
		/// <param name="type">Token type</param>
		/// <param name="charIndex">The index in the string</param>
		/// <param name="output">The output token vector</param>
		inline void PushToken(TokenType type);

		/// <summary>
		/// Given the starting character, decide the parse state
		/// </summary>
		/// <param name="c">Starting char</param>
		/// <returns>New parsestate</returns>
		static inline ParseState GetParseState(char c);

		/// <summary>
		/// The function to run for each state, index is state
		/// </summary>
		static const std::vector<StateFunction> sStateFunction;

		/// <summary>
		/// The map from charcater to parse state for non-digit-non-alphabet characters
		/// </summary>
		static const std::map<char, ParseState> sCharacterStateMap;

		/// <summary>
		/// The map from a string to C++ keyword type
		/// </summary>
		static const std::unordered_map<std::string, TokenType> sKeywordMap;

		inline void NewToken(char c);
		void SpaceState(char c);
		void IgnoreState(char c);
		void WordState(char c);
		void DigitState(char c);
		void LeftParenState(char c);
		void RightParenState(char c);
		void LeftBracketState(char c);
		void RightBracketState(char c);
		void SemicolonState(char c);
		void AssignState(char c);
		void QuoteState(char c);
		void LeftAngleBracketState(char c);
		void RightAngleBracketState(char c);
		void StarState(char c);
		void AmpersandState(char c);
		void ColonState(char c);
		void DoubleColonState(char c);
		void SlashState(char c);
		void DoubleSlashState(char c);
		void SlashStar(char c);
		void EndStar(char c);
	};
}