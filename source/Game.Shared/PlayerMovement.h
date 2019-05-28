#pragma once
#include "Reaction.h"
#include "Factory.h"

namespace GameEngine
{
	class Player;

	class PlayerMovement final : public Reaction
	{
		RTTI_DECLARATIONS(PlayerMovement, Reaction)
	public:
		/// <summary>
		/// Creates a player movement component
		/// </summary>
		explicit PlayerMovement(RTTI::IdType Type = PlayerMovement::TypeIdClass());
		~PlayerMovement();

		/// <summary>
		/// Override Update method of ActionList so this one does nothing
		/// </summary>
		/// <param name="state">WorldState object</param>
		//virtual void Update(WorldState& state) override;

		/// <summary>
		/// Interface for Clone. To be implemented in derived class
		/// </summary>
		/// <returns>A clone of this reaction object</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Interface for event notify. To be implemented in derived class
		/// </summary>
		/// <param name="event">The event object that triggers the notify</param>
		virtual void Notify(const BaseEvent& event) override;

		/// <summary>
		/// List of signatures for this type. This just returns the signature list of ActionList
		/// </summary>
		/// <returns>Signature list of this type</returns>
		static const Vector<Attributed::Signature> Signatures();

		void SetParentPlayer(Player* ParentPlayer);

	private:
		float m_MovementSpeed = 5.f;
		glm::vec4 m_CurrentForwardVector;
		glm::vec4 m_CurrentPlayerMove;
		Player* m_ParentPlayer;
		std::chrono::time_point<std::chrono::steady_clock> m_LastShootTime;
	};

	DECLARE_FACTORY(PlayerMovement, Scope)
}