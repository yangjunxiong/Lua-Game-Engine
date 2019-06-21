#pragma once
#include "RTTI.h"

namespace GameEngine
{
	class World;
	class Sector;
	class Entity;
	class Action;
	class GameTime;

	CLASS();
	/// <summary>
	/// A WorldState is simply the data carrier throughout a game loop, so that each participant can easilly access data
	/// </summary>
	class WorldState final : public RTTI
	{
		RTTI_DECLARATIONS(WorldState, RTTI);
		friend World;
		friend Sector;
		friend Entity;
		friend Action;

	public:
		/// <summary>
		/// Default constructor. Takes a external GameTime and stores it
		/// </summary>
		/// <param name="time">External GameTime object</param>
		WorldState(const GameTime& time);

		/// <summary>
		/// Get stored GameTime
		/// </summary>
		/// <returns>GameTime object</returns>
		const GameTime& GetGameTime() const;

		FUNCTION();
		/// <summary>
		/// Get the World currently being updated
		/// </summary>
		/// <returns>The World currently being updated</returns>
		World* GetWorld();

		FUNCTION();
		/// <summary>
		/// Get the Sector currently being updated
		/// </summary>
		/// <returns>The Sector currently being updated</returns>
		Sector* GetSector();

		FUNCTION();
		/// <summary>
		/// Get the Entity currently being updated
		/// </summary>
		/// <returns>The Entity currently being updated</returns>
		Entity* GetEntity();

		FUNCTION();
		/// <summary>
		/// Get the Action currently being updated
		/// </summary>
		/// <returns>The Action currently being updated</returns>
		Action* GetAction();

	private:
		/// <summary>
		/// GameTime reference to an external object
		/// </summary>
		const GameTime* mTime;

		/// <summary>
		/// World currently being updated
		/// </summary>
		World* mWorld = nullptr;

		/// <summary>
		/// Sector currently being updated
		/// </summary>
		Sector* mSector = nullptr;

		/// <summary>
		/// Entity currently being updated
		/// </summary>
		Entity* mEntity = nullptr;

		/// <summary>
		/// Action currently being updated
		/// </summary>
		Action* mAction = nullptr;
	};
}