#pragma once
#include "Action.h"

namespace GameEngine
{
	/// <summary>
	/// Destroy an action by its name.
	/// It will search starting from this action up until it reaches Entity, but will not search for siblings
	/// </summary>
	class ActionDestroyAction : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="type">Type to search for signature list</param>
		explicit ActionDestroyAction(RTTI::IdType type = ActionDestroyAction::TypeIdClass());

		/// <summary>
		/// Destroy the action by name
		/// </summary>
		/// <param name="state">WorldState object</param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Return a copy of this object
		/// </summary>
		/// <returns>A copy of this object</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Return the signature list
		/// </summary>
		/// <returns>Signature list</returns>
		static const Vector<Attributed::Signature> Signatures();

		/// <summary>
		/// The name of datum storing target action name
		/// </summary>
		static inline const std::string TARGET_KEY = "Target";

	protected:
		/// <summary>
		/// The name of the action to destroy
		/// </summary>
		std::string mTarget;
	};

	DECLARE_FACTORY(ActionDestroyAction, Scope);
}
