#pragma once
#include "Action.h"

namespace GameEngine
{
	class ActionUpdateHealthUI : public Action
	{
		RTTI_DECLARATIONS(ActionUpdateHealthUI, Action)
	public:
		ActionUpdateHealthUI();
		~ActionUpdateHealthUI();

		/// <summary>
		/// Update method exectued every frame
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
	};
}
