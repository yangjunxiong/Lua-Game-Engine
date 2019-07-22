#pragma once
#include "Attributed.h"
#include "Factory.h"

namespace GameEngine
{
	class Entity;
	class WorldState;

	CLASS(NoLuaAuthority);
	/// <summary>
	/// Action represents the logic to perform on entities
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);

	public:
		Action(Entity* parent);

		/// <summary>
		/// Update method exectued every frame
		/// </summary>
		/// <param name="state">WorldState object</param>
		virtual void Update(WorldState& state) = 0;

		virtual void Start(WorldState&) {};

		/// <summary>
		/// Get the name of the action
		/// </summary>
		/// <returns>The name of the action</returns>
		FUNCTION();
		const std::string& Name() const;

		/// <summary>
		/// Set the name of the action
		/// </summary>
		/// <param name="name">New name of the action</param>
		FUNCTION();
		void SetName(const std::string& name);

		/// <summary>
		/// Return a copy of this object
		/// </summary>
		/// <returns>A copy of this object</returns>
		virtual gsl::owner<Scope*> Clone() const override = 0;

		/// <summary>
		/// Return the signature list
		/// </summary>
		/// <returns>Signature list</returns>
		static const Vector<Attributed::Signature> Signatures();

	protected:
		/// <summary>
		/// Default constructor. Make it protected so Action is an abstract class
		/// </summary>
		/// <param name="type">Type to search for signature list</param>
		explicit Action(Entity* parent, RTTI::IdType type = Action::TypeIdClass());

		/// <summary>
		/// Name of the Action
		/// </summary>
		std::string mName;

		Entity* mParent = nullptr;
	};
}