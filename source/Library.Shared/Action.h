#pragma once
#include "Attributed.h"
#include "Factory.h"

namespace GameEngine
{
	class Entity;
	class WorldState;

	CLASS();
	/// <summary>
	/// Action represents the logic to perform on entities
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);

	public:
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
		/// Search for the action with given name up from ancestor tree. It either ends at root Entity or the object without parent
		/// Will not search downwards from this Action, will not drill down to siblings as well
		/// </summary>
		/// <param name="name">Name of the action to search</param>
		/// <returns>The action pointer if found, nullptr otherwise</returns>
		Action* SearchAction(const std::string& name);

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
		explicit Action(RTTI::IdType type = Action::TypeIdClass());

		/// <summary>
		/// Name of the Action
		/// </summary>
		std::string mName;
	};

	/// <summary>
	/// ActionList is a composite that contains many actions
	/// </summary>
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="type">Type to search for signature list</param>
		explicit ActionList(RTTI::IdType type = ActionList::TypeIdClass());

		/// <summary>
		/// Return the datum storing the action list
		/// </summary>
		/// <returns>The datum storing the action list</returns>
		Datum& GetActionList();

		/// <summary>
		/// Return the datum storing the action list
		/// </summary>
		/// <returns>The datum storing the action list</returns>
		const Datum& GetActionList() const;

		/// <summary>
		/// Create an action and make this entity its parent
		/// </summary>
		/// <param name="className">Class name of the action to create</param>
		/// <param name="instanceName">Instance name of the new action</param>
		/// <returns>The newly created action</returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Update each action in the list
		/// </summary>
		/// <param name="state">WorldState object</param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Get a copy from this object
		/// </summary>
		/// <returns>A copy of this object</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Return the signature list
		/// </summary>
		/// <returns>Signature list</returns>
		static const Vector<Attributed::Signature> Signatures();

		/// <summary>
		/// The key name for action list datum
		/// </summary>
		static inline const std::string ACTION_LIST_TABLE_KEY = "ActionList";

		/// <summary>
		/// The index for action list datum
		/// </summary>
		static inline const size_t ACTION_LIST_TABLE_INDEX = 2;
	};

	DECLARE_FACTORY(ActionList, Scope);
}