#pragma once
#include "Entity.h"
#include "Factory.h"
#include "PlayerMovement.h"

namespace GameEngine
{
	class Player final : public Entity
	{
		RTTI_DECLARATIONS(Player, Entity)
	public:
		Player();
		Player(int IDNumber);
		~Player();

		virtual void Start(WorldState& state) override;

		virtual gsl::owner<Scope*> Clone() const override;

		static const Vector<Attributed::Signature> Signatures();

		void SetPlayerID(int ID);

		int GetPlayerID() const;

		void SetHealth(int Health);

		int GetHealth() const;

		int GetCooldown() const;

		void TakeDamage(int damage, WorldState& state);

	private:
		PlayerMovement m_PlayerMovement;
		void Die(WorldState& state);

		int mShootCooldown = 1000;
		int m_Health = 3;
		int PlayerID;
		class ActionAnimation* mAnimation = nullptr;
	};

	DECLARE_FACTORY(Player, Scope)
}