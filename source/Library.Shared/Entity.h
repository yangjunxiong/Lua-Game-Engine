#pragma once
#include "Attributed.h"
#include "Factory.h"
//#include "Material.h"
#include <glm/fwd.hpp>
#include "Transform.h"
#include "World.h"

namespace GameEngine
{
	class Sector;
	class Action;
	class WorldState;

	CLASS();
	/// <summary>
	/// An entity represents a base individual object in the game.
	/// It can contain actions
	/// </summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed);

	public:
		CONSTRUCTOR();
		/// <summary>
		/// Default constructor, builds a free entity object without parent or child
		/// </summary>
		Entity();

		/// <summary>
		/// Default constructor. Takes a type argument for inheriting purpose
		/// </summary>
		/// <param name="type">Actual type of object</param>
		explicit Entity(RTTI::IdType type);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">Other Entity to copy from</param>
		Entity(const Entity& other) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">Other Entity to move from</param>
		Entity(Entity&& other) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="other">Other Entity to copy from</param>
		/// <returns>This Entity after copying</returns>
		Entity& operator=(const Entity& other) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">Other Entity to move from</param>
		/// <returns>This Entity after moving</returns>
		Entity& operator=(Entity&& other) = default;

		/// <summary>
		/// Default desctructor
		/// </summary>
		virtual ~Entity() = default;

		FUNCTION();
		/// <summary>
		/// Get the name of this Entity
		/// </summary>
		/// <returns>The name of this Entity</returns>
		const std::string& Name() const;

		FUNCTION();
		/// <summary>
		/// Set the name of this Entity
		/// </summary>
		/// <param name="name">New name of this Entity</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Set the Sector this Entity belongs to. Will remove this Entity from previous Sector
		/// </summary>
		/// <param name="sector">The parent Sector</param>
		void SetSector(Sector& sector);

		/// <summary>
		/// Get the Datum of actions in this entity
		/// </summary>
		/// <returns>Datum storing list of actions</returns>
		Datum& Actions();

		/// <summary>
		/// Get the Datum of actions in this entity
		/// </summary>
		/// <returns>Datum storing list of actions</returns>
		const Datum& Actions() const;

		Transform& GetTransform();
		const Transform& GetTransform() const;
		void SetTransform(const Transform& trans);

		FUNCTION();
		glm::vec4 GetPosition() const;

		FUNCTION();
		glm::vec4 GetRotation() const;

		FUNCTION();
		glm::vec4 GetScale() const;

		FUNCTION()
		bool IsActive() const;

		FUNCTION();
		void SetPosition(const glm::vec4& pos);

		FUNCTION();
		void SetRotation(const glm::vec4& rot);

		FUNCTION();
		void SetScale(const glm::vec4& scale);

		FUNCTION()
		void SetActive(bool active);

		/// <summary>
		/// Create an action and make this entity its parent
		/// </summary>
		/// <param name="className">Class name of the action to create</param>
		/// <param name="instanceName">Instance name of the new action</param>
		/// <returns>The newly created action</returns>
		Action* CreateAction(std::string className, std::string instanceName);

		/// <summary>
		/// Called once after constructed and configured, and before any update
		/// </summary>
		/// <param name="state"></param>
		virtual void Start(WorldState& state);

		/// <summary>
		/// Update this Entity
		/// </summary>
		/// <param name="state">The WorldState</param>
		virtual void Update(WorldState& state);

		/// <summary>
		/// Draw the entity using passed-in function, this is to support different rendering method like OpenGL and DirectX
		/// </summary>
		void Draw(const class World::RenderFunction& func);

		/// <summary>
		/// Create a copy of this Entity object
		/// </summary>
		/// <returns>A copy of this object</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Return signature list to populate attributes
		/// </summary>
		/// <returns>Signature list for Entity type</returns>
		static const Vector<Attributed::Signature> Signatures();

		PROPERTY();
		/// <summary>
		/// The special key in Scope that maps to the actions datum
		/// </summary>
		static inline const std::string ACTION_TABLE_KEY = "Actions";

		PROPERTY();
		/// <summary>
		/// The special index in Scope that maps to the actions datum
		/// </summary>
		static inline const size_t ACTION_TABLE_INDEX = 6;


	protected:
		/// <summary>
		/// Transform of this entity
		/// </summary>
		Transform mTransform;

		/// <summary>
		/// Name of the Entity
		/// </summary>
		std::string mName;

		/// <summary>
		/// Whether this entity is active or not, inactive entity will not be updated and will not rendered either
		/// </summary>
		int mActive = 1;
	};

	DECLARE_FACTORY(Entity, Scope);
}