#pragma once
#include "Action.h"

namespace GameEngine
{
	class WorldState;

	/// <summary>
	/// Flow control logic if.
	/// Will check the condition in "Condition" key and execute the first action if condition is not 0, second action if condition is 0
	/// </summary>
	class ActionListIf : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="type">Type to search for signature list</param>
		explicit ActionListIf(RTTI::IdType type = ActionListIf::TypeIdClass());

		/// <summary>
		/// Execute the check of condition and execute the corresponding action
		/// </summary>
		/// <param name="state">WorldState object</param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Get a copy of this object
		/// </summary>
		/// <returns>A copy of this object</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Return the signature list
		/// </summary>
		/// <returns>Signature list</returns>
		static const Vector<Attributed::Signature> Signatures();

		/// <summary>
		/// The name of the datum storing the condition
		/// </summary>
		static inline const std::string CONDITION_KEY = "Condition";

	protected:
		/// <summary>
		/// The condition to check to decide which branch action to execute
		/// </summary>
		int mCondition = 0;
	};

	DECLARE_FACTORY(ActionListIf, Scope);
}
