#pragma once
#include "vector.h"
#include "HashMap.h"
#include <functional>

namespace GameEngine
{
	/// <summary>
	/// Tokenize the expression string and convert it to RPN notation
	/// </summary>
	class Tokenizer final
	{
	public:
		/// <summary>
		/// Type of token
		/// </summary>
		enum class TokenType
		{
			Integer,
			Float,
			Variable,
			StringLiteral,
			LeftParen,
			RightParen,

			// Binary operator
			Assign,
			Add,
			Minus,
			Multiply,
			Divide,
			Mod,
			BoolAnd,
			BoolOr,

			// Unary operator
			Negate,
			Not,

			// Comparison
			Equal,
			NotEqual,
			GreaterThan,
			GreaterEqual,
			LessThan,
			LessEqual
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

			Token(const std::string& str, TokenType type);

			/// <summary>
			/// Check if this token is an operator type
			/// </summary>
			/// <returns>True if the token is operator type</returns>
			bool IsOperator() const;

			/// <summary>
			/// Check if this token is unary operator
			/// </summary>
			/// <returns>True if the token is unary operator</returns>
			bool IsUnaryOperator() const;
		};

		/// <summary>
		/// Given an expression string, tokenize it and fill the output vector
		/// </summary>
		/// <param name="input">Expression string</param>
		/// <param name="output">Output vector of tokens, should be empty when passed in</param>
		static void Tokenize(const std::string& input, Vector<Token>& output);

		/// <summary>
		/// Given an array of tokens, convert it to RPN notation and fill the output vector
		/// </summary>
		/// <param name="input">Array of tokens</param>
		/// <param name="output">Output vector of tokens in RPN order, shoudl be empty when passed in</param>
		static void ConvertToRPN(const Vector<Token>& input, Vector<Token>& output);

	private:
		/// <summary>
		/// The state of parsing state machine
		/// </summary>
		enum class ParseState
		{
			Empty = 0,
			Digit,
			Dot,            // "." for float number
			FloatDigit,     // the part after "."
			Variable,

			Plus,
			Minus,
			Multiply,
			Divide,
			Assign,
			LeftParen,
			RightParen,
			Quote,
			Mod,
			Not,
			Greater,
			Less,
			BoolAnd,
			BoolOr
		};

		/// <summary>
		/// Check if the char is digit
		/// </summary>
		/// <param name="c">input char</param>
		/// <returns>True if the char is digit</returns>
		static inline bool IsDigit(const char& c);

		/// <summary>
		/// Check if the char is alphabetical letter
		/// </summary>
		/// <param name="c">input char</param>
		/// <returns>True if the char is letter</returns>
		static inline bool IsLetter(const char& c);

		/// <summary>
		/// Compare the precedence of two operators
		/// </summary>
		/// <param name="token1">First operator</param>
		/// <param name="token2">Second operator</param>
		/// <returns>1 if first operator is higher than second, 0 if they are equal, -1 if first operator is lower than second</returns>
		static inline int ComparePrecedence(const Token& token1, const Token& token2);

		/// <summary>
		/// Get the precedence of an operator
		/// </summary>
		/// <param name="token">Operator</param>
		/// <returns>The precedence of operator</returns>
		static inline int GetPrecedence(const Token& token);

		/// <summary>
		/// Push a parsed token to the token vector
		/// </summary>
		/// <param name="buffer">String of token</param>
		/// <param name="type">Token type</param>
		/// <param name="charIndex">The index in the string</param>
		/// <param name="output">The output token vector</param>
		static inline void PushToken(char* buffer, TokenType type, size_t& charIndex, Vector<Token>& output);

		/// <summary>
		/// Given the starting character, decide the parse state
		/// </summary>
		/// <param name="c">Starting char</param>
		/// <returns>New parsestate</returns>
		static inline ParseState GetParseState(char c);
		static void EmptyState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void DigitState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void DotState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void FloatDigitState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void VariableState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void PlusState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void MinusState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void MultiplyState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void DivideState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void AssignState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void LeftParenState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void RightParenState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void QuoteState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void ModState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void NotState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void GreaterState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void LessState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void BoolAndState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		static void BoolOrState(ParseState& state, char* buffer, size_t& charIndex, char newChar, Vector<Token>& output);
		
		using StateFunction = void (*) (ParseState&, char*, size_t&, char, Vector<Token>&);

		/// <summary>
		/// The function to run for each state, index is state
		/// </summary>
		static Vector<StateFunction> sStateFunction;

		/// <summary>
		/// The map from charcater to parse state for non-digit-non-alphabet characters
		/// </summary>
		static HashMap<char, ParseState> sCharacterStateMap;
	};
}