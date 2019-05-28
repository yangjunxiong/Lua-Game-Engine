#pragma once
#include "Action.h"
#include "Tokenizer.h"
#include "Datum.h"

namespace GameEngine
{
	/// <summary>
	/// Perform C style expression in update.
	/// Normally is used to assign a value to some variable, even though you can still just do expression without assigning the result to anywhere
	/// </summary>
	class ActionExpression final : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		ActionExpression();

		/// <summary>
		/// Perfome the expression
		/// </summary>
		/// <param name="state"></param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Get the copy of this object
		/// </summary>
		/// <returns>The clone of this object</returns>
		virtual gsl::owner<ActionExpression*> Clone() const override;

		/// <summary>
		/// Set the expression to a new one, will set the exprssion as dirty and hence will be evaluated in next update
		/// </summary>
		/// <param name="exp">New expression string</param>
		void SetExpression(const std::string& exp);

		/// <summary>
		/// Set the expression to a new one, will set the exprssion as dirty and hence will be evaluated in next update
		/// </summary>
		/// <param name="exp">New expression string</param>
		void SetExpression(std::string&& exp);

		/// <summary>
		/// Get the expression string
		/// </summary>
		/// <returns>The expression string</returns>
		const std::string& GetExpression() const;

		/// <summary>
		/// Get the signature list of this type
		/// </summary>
		/// <returns>List of signatures</returns>
		static const Vector<Attributed::Signature> Signatures();

	private:
		/// <summary>
		/// Raw string of expression
		/// </summary>
		std::string mExpression;

		/// <summary>
		/// Parse expression stored as RPN notation tokens
		/// </summary>
		Vector<Tokenizer::Token> mTokens;

		/// <summary>
		/// Whether the raw expression is dirty and needs reevaluate
		/// </summary>
		bool mDirty = true;

		/// <summary>
		/// Helper struct to make working with dynamic tokens easier
		/// </summary>
		struct Operand
		{
			/// <summary>
			/// The datum storing the actual value for the token in expression.
			/// Might be unknown for variable token type, and will be evaluated at run-time
			/// </summary>
			Datum mDatum;

			/// <summary>
			/// The raw string of token
			/// </summary>
			std::string mString;

			Operand(const Datum& datum) : mDatum(datum) {};
			Operand(Datum&& datum) : mDatum(std::move(datum)) {};
			Operand(const std::string& str) : mString(str) {};
		};

		/// <summary>
		/// If raw expression string is dirty, tokenize it and convert the tokens to RPN notation
		/// </summary>
		void Evaluate();

		/// <summary>
		/// At run-time in Update function, evaluate the operand and fill in datum values for tokens with variable type
		/// </summary>
		/// <param name="operand">The operand to fill in</param>
		void FillOperand(Operand& operand) const;
	};

	DECLARE_FACTORY(ActionExpression, Scope);
}
