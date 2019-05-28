#pragma once
#include "Action.h"

namespace GameEngine
{
	/// <summary>
	/// Create another action by class name
	/// </summary>
	class ActionCreateAction : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="type">Type to search for signature list</param>
		explicit ActionCreateAction(RTTI::IdType type = ActionCreateAction::TypeIdClass());

		/// <summary>
		/// Create a new action
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
		/// The name of the datum storing class name
		/// </summary>
		static inline const std::string CLASS_KEY = "Class";

		/// <summary>
		/// The name of the datum storing instance name
		/// </summary>
		static inline const std::string INSTANCE_NAME_KEY = "InstanceName";

	protected:
		/// <summary>
		/// Name of the class to create
		/// </summary>
		std::string mClass;

		/// <summary>
		/// Name of newly created instance
		/// </summary>
		std::string mInstanceName;
	};

	DECLARE_FACTORY(ActionCreateAction, Scope);
}
