#pragma once
#include "Attributed.h"
#include "Factory.h"
#include "World.h"
//#include "Material.h"

namespace GameEngine
{
	class Entity;
	class WorldState;

	/// <summary>
	/// An entity represents a group of objects in the world. Equivalent to a game level
	/// It can contain many Entity
	/// </summary>
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		Sector();

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">Other Sector to copy from</param>
		Sector(const Sector& other) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">Other Sector to move from</param>
		Sector(Sector&& other) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="other">Other Sector to copy from</param>
		/// <returns>This Sector after copying</returns>
		Sector& operator=(const Sector& other) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">Other Sector to move from</param>
		/// <returns>This Sector after moving</returns>
		Sector& operator=(Sector&& other) = default;

		/// <summary>
		/// Default desctructor
		/// </summary>
		~Sector() = default;

		/// <summary>
		/// Get the name of this Sector
		/// </summary>
		/// <returns>The name of this Sector</returns>
		const std::string& Name() const;

		/// <summary>
		/// Set the name of this Sector
		/// </summary>
		/// <param name="name">New name of this Sector</param>
		void SetName(const std::string& name);

		void SetActive(bool active);
		bool IsActive() const;

		/// <summary>
		/// Get the Datum containing the Entity
		/// </summary>
		/// <returns>The Datum containing the Entity</returns>
		Datum& Entities();

		/// <summary>
		/// Get the Datum containing the Entity
		/// </summary>
		/// <returns>The Datum containing the Entity</returns>
		const Datum& Entities() const;

		/// <summary>
		/// Create a new Entity with given name and make it a child of this Sector
		/// </summary>
		/// <param name="name">Name of new Entity</param>
		/// <returns>The Entity created</returns>
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Set the World this Sector belongs to. Will remove this Sector from previous World
		/// </summary>
		/// <param name="sector">The parent World</param>
		void SetWorld(World& world);

		/// <summary>
		/// Called once after constructed from Json
		/// </summary>
		void Start(WorldState& state);

		/// <summary>
		/// Update all Entity with given WorldState
		/// </summary>
		/// <param name="state">The WorldState</param>
		void Update(WorldState& state);

		/// <summary>
		/// Draw the sector, which will call draw on each child entity
		/// </summary>
		void Draw(const class World::RenderFunction& func);

		/// <summary>
		/// Create a copy of this Sector
		/// </summary>
		/// <returns>The copy of this Sector</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Return signature list to populate attributes
		/// </summary>
		/// <returns>Signature list for Sector type</returns>
		static const Vector<Attributed::Signature> Signatures();

		/// <summary>
		/// The special key in Scope that maps to the Entity datum
		/// </summary>
		inline static const std::string ENTITY_TABLE_KEY = "Entities";

		/// <summary>
		/// The special index in Scope that maps to the Entity datum
		/// </summary>
		inline static const size_t ENTITY_TABLE_INDEX = 3;

	private:
		/// <summary>
		/// Name of the Sector
		/// </summary>
		std::string mName;

		/// <summary>
		/// Whether the section is active
		/// </summary>
		int mActive = 1;
	};

	DECLARE_FACTORY(Sector, Scope);
}