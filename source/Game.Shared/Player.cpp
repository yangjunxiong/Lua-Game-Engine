#include "pch.h"
#include "Player.h"
#include "../Library.Shared/WorldState.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "ActionAnimation.h"

namespace GameEngine
{
	RTTI_DEFINITIONS(Player)

	Player::Player() : Entity(Player::TypeIdClass())
	{
		m_PlayerMovement.SetParentPlayer(this);
	}

	Player::Player(int IDNumber) : Entity(Player::TypeIdClass()), PlayerID(IDNumber)
	{
		m_PlayerMovement.SetParentPlayer(this);
	}
	
	Player::~Player()
	{
	}

	void Player::Start(WorldState&)
	{
		Datum& actions = Actions();
		for (size_t i = 0; i < actions.Size(); ++i)
		{
			Scope& scope = actions[int(i)];
			if (scope.Is(ActionAnimation::TypeIdClass()))
			{
				mAnimation = static_cast<ActionAnimation*>(&scope);
				mAnimation->SetAnimState("Moving");
			}
		}
	}

	gsl::owner<Scope*> Player::Clone() const
	{
		return new Player(*this);
	}

	const Vector<Attributed::Signature> Player::Signatures()
	{
		return Vector<Attributed::Signature>(
			{
				Signature("Health", Datum::DatumType::Integer, 1, offsetof(Player, m_Health)),
				Signature("PlayerID", Datum::DatumType::Integer, 1, offsetof(Player, PlayerID)),
				Signature("Cooldown", Datum::DatumType::Integer, 1, offsetof(Player, mShootCooldown))
			});
	}

	void Player::SetPlayerID(int ID)
	{
		PlayerID = ID;
	}

	int Player::GetPlayerID() const
	{
		return PlayerID;
	}

	void Player::SetHealth(int Health)
	{
		m_Health = Health;
	}

	int Player::GetHealth() const
	{
		return m_Health;
	}

	int Player::GetCooldown() const
	{
		return mShootCooldown;
	}

	void Player::TakeDamage(int damage, WorldState& state)
	{
		if (m_Health > 0)
		{
			// Take damage
			int oldHealth = m_Health;
			m_Health -= damage;

			// Send take damage event
			EventMessageAttributed message;
			message["Subtype"] = "PlayerDamage";
			message["WorldState"] = &state;
			message["PlayerId"] = PlayerID;
			message["OldHealth"] = oldHealth;
			message["NewHealth"] = m_Health;
			shared_ptr<Event<EventMessageAttributed>> event = make_shared<Event<EventMessageAttributed>>(message);
			state.GetWorld()->EnqueueEvent(event);

			// Process death
			if (m_Health <= 0)
			{
				Die(state);
			}
		}
	}

	void Player::Die(WorldState& state)
	{
		if (mAnimation != nullptr)
		{
			mAnimation->SetAnimState("Death");
		}

		EventMessageAttributed message;
		message["Subtype"] = "PlayerDie";
		message["WorldState"] = &state;
		message["PlayerId"] = PlayerID;
		shared_ptr<Event<EventMessageAttributed>> event = make_shared<Event<EventMessageAttributed>>(message);
		state.GetWorld()->EnqueueEvent(event);
	}
}