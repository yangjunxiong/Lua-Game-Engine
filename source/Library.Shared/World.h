#pragma once
#include "Attributed.h"
#include "Factory.h"
#include "WorldState.h"
#include "EventQueue.h"
#include "ActionRender.h"
#include "Transform.h"
#include <functional>

namespace GameEngine
{
	class Sector;
	CLASS();
	/// <summary>
	/// A World is the container for all objects in the game. It contains many sectors and will update each one of them in each tick
	/// </summary>
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);

	public:
		/// <summary>
		/// Default constructor. Takes a GameTime object and constructs WorldState with it
		/// </summary>
		/// <param name="time">External GameTime object</param>
		explicit World(const GameTime& time);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">Other World object to copy from</param>
		World(const World& other) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">Other World object to move from</param>
		World(World&& other) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="other">Other World object to copy from</param>
		/// <returns>This object after copying</returns>
		World& operator=(const World& other) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">Other World object to move from</param>
		/// <returns>This object after moving</returns>
		World& operator=(World&& other) = default;

		/// <summary>
		/// Default constructor
		/// </summary>
		~World() = default;

		/// <summary>
		/// Get the name of the World
		/// </summary>
		/// <returns></returns>
		const std::string& Name() const;

		/// <summary>
		/// Set the name of the World
		/// </summary>
		/// <param name="name">Name of the World</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Get the Datum containing the sectors
		/// </summary>
		/// <returns>The Datum containing the sectors</returns>
		Datum& Sectors();

		/// <summary>
		/// Get the Datum containing the sectors
		/// </summary>
		/// <returns>The Datum containing the sectors</returns>
		const Datum& Sectors() const;

		FUNCTION();
		/// <summary>
		/// Create a new sector with given name and make it a child of this World
		/// </summary>
		/// <param name="name">Name of new sector</param>
		/// <returns>The Sector created</returns>
		Sector* CreateSector(const std::string& name);

		FUNCTION();
		/// <summary>
		/// Try to destroy an attributed object by putting it into the destroying queue, actual destruction will happend at the end of frame
		/// </summary>
		/// <param name="action">The object to destroy, it should be heap allocated object, otherwise will blowup</param>
		void Destroy(Attributed& object);

		/// <summary>
		/// Called once after constructed from Json
		/// </summary>
		void Start();

		/// <summary>
		/// Update all sectors. This will use this World's WorldState
		/// </summary>
		void Update();

		using RenderFunction = std::function<void(const Transform&, const ActionRender::RenderParam&)>;
		/// <summary>
		/// Draw the world, will pass the function down and use it to draw each sector
		/// </summary>
		/// <param name="func">The function to perform drawing</param>
		void Draw(const RenderFunction& func);

		/// <summary>
		/// Update all sectors with given WorldState
		/// </summary>
		/// <param name="state">An external WorldState</param>
		void Update(WorldState& state);

		/// <summary>
		/// Add an event to the event queue
		/// </summary>
		/// <param name="event">Shared pointer to the event</param>
		/// <param name="delay">Delay in milliseconds before sending the event</param>
		void EnqueueEvent(const std::shared_ptr<BaseEvent>& event, const std::chrono::milliseconds& delay = std::chrono::milliseconds(0));

		/// <summary>
		/// Remove an event from the event queue
		/// </summary>
		/// <param name="event">Shared pointer to the event to remove</param>
		void DequeueEvent(const std::shared_ptr<BaseEvent>& event);

		/// <summary>
		/// Get the event queue. This is const only because we don't want to game code to change the event queue
		/// </summary>
		/// <returns>The world event queue</returns>
		const EventQueue& GetEventQueue() const;

		/// <summary>
		/// Create a copy of this World
		/// </summary>
		/// <returns>The copy of this World</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Return signature list to populate attributes
		/// </summary>
		/// <returns>Signature list for World type</returns>
		static const Vector<Attributed::Signature> Signatures();

		/// <summary>
		/// The special key in Scope that maps to the sectors datum
		/// </summary>
		inline static const std::string SECTOR_TABLE_KEY = "Sectors";

		/// <summary>
		/// The special index in Scope that maps to the sectors datum
		/// </summary>
		inline static const size_t SECTOR_TABLE_INDEX = 2;

		inline static const std::string ACTION_TABLE_KEY = "Actions";
		inline static const size_t ACTION_TABLE_INDEX = 3;

		Datum& Actions();
		const Datum& Actions() const;

		FUNCTION();
		WorldState* GetWorldState() { return &mState; };

	private:
		/// <summary>
		/// Name of the world
		/// </summary>
		std::string mName;

		/// <summary>
		/// WorldState of this world
		/// </summary>
		WorldState mState;

		/// <summary>
		/// Event queue to the world
		/// </summary>
		EventQueue mEventQueue;

		/// <summary>
		/// The list of pending kill objects, will be handled at the end of each frame
		/// </summary>
		Vector<Attributed*> mDestroyQueue;
	};
}