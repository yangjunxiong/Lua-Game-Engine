#pragma once
#include "Action.h"
#include "Factory.h"

namespace GameEngine
{
	/// <summary>
	/// Increment an integer or float by certain step
	/// </summary>
	class ActionIncrement :	public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="type">Type to search for signature list</param>
		explicit ActionIncrement(RTTI::IdType type = ActionIncrement::TypeIdClass());

		/// <summary>
		/// Increment the given field by step
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
		/// Name of the datum storing target name
		/// </summary>
		static inline const std::string TARGET_KEY = "Target";

		/// <summary>
		/// Name of the datum storing step name
		/// </summary>
		static inline const std::string STEP_KEY = "Step";

	protected:
		/// <summary>
		/// Name of the target attribute to increment
		/// </summary>
		std::string mTargetName;

		/// <summary>
		/// Value to increment
		/// </summary>
		float mStep = 1.f;
	};

	DECLARE_FACTORY(ActionIncrement, Scope);
}
