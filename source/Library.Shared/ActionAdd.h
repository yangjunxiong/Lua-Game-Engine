#pragma once
#include "Action.h"

namespace GameEngine
{
	/// <summary>
	/// ActionAdd adds multiple homogenious values specified in "Operands" field and put the result in "Result" field.
	/// It supports integer, float, and string add. However all operands in the action must be of the same type, otherwise will throw exception.
	/// The result's type is determined based on the operand type, so it's not a prescribed attribute.
	/// Operand is table type and contains two attributes:
	///		"Type"[integer]:
	///			0 - Use the value in "Value" directly. In this case, "Value" should be of either integer, float, or string type
	///			1 - Use the value in "Value" as the key to search for other datum. In this case, "Value" should be string type
	///		"Value"[integer/float/string]: Store the value specified in "Type" attribute
	/// </summary>
	class ActionAdd final : public Action
	{
		RTTI_DECLARATIONS(ActionAdd, Action);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		ActionAdd();

		/// <summary>
		/// Add all operands together and put the value in "Result" attribute
		/// </summary>
		/// <param name="state">WorldState object</param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Get a clone of this action
		/// </summary>
		/// <returns>A clone of this action</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Get the list of operands
		/// </summary>
		/// <returns>The list of operands</returns>
		Datum& Operands();

		/// <summary>
		/// Get the list of operands
		/// </summary>
		/// <returns>The list of operands</returns>
		const Datum& Operands() const;

		/// <summary>
		/// Get the result datum
		/// </summary>
		/// <returns>The result datum</returns>
		const Datum& Result() const;

		/// <summary>
		/// Get the list of signatures for this type
		/// </summary>
		/// <returns>The list of signatures for this type</returns>
		static const Vector<Attributed::Signature> Signatures();

		/// <summary>
		/// Key for operands list
		/// </summary>
		static inline const std::string OPERAND_KEY = "Operands";

		/// <summary>
		/// Index for operands list
		/// </summary>
		static inline const size_t OPERAND_INDEX = 2;

		/// <summary>
		/// Key for result attributed. Result has no index since it's not a prescribed attribute
		/// </summary>
		static inline const std::string RESULT_KEY = "Result";
	};

	DECLARE_FACTORY(ActionAdd, Scope);
}
