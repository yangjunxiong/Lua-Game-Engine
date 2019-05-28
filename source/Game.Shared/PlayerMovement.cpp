#include "pch.h"
#include "PlayerMovement.h"
#include "Event.h"
#include "../Library.Desktop/EventInput.h"
#include "../Library.Desktop/InputManager.h"
#include "glm/glm.hpp"
#include "Player.h"
#include "GameTime.h"
#include <chrono>
#include "Bullet.h"
#include "ActionLoadJson.h"
#include "Sector.h"

namespace GameEngine
{
	RTTI_DEFINITIONS(PlayerMovement)

	PlayerMovement::PlayerMovement(RTTI::IdType Type) : Reaction(Type)
	{
		Event<EventInput>::Subscribe(*this);
	}

	PlayerMovement::~PlayerMovement()
	{
		Event<EventInput>::Unsubscribe(*this);
	}

	gsl::owner<Scope*> PlayerMovement::Clone() const
	{
		return new PlayerMovement(*this);
	}

	void PlayerMovement::Notify(const BaseEvent& event)
	{
		if (m_ParentPlayer->GetHealth() <= 0)
		{
			return;
		}

		/*
		 * This block gets the most recent input event, gets the player's inputs,
		 * and sets the player's current forward vector and position. In turn,
		 * this sets up a call to the rendering and collision systems to update
		 * the player's view in-game
		 */
		assert(event.Is(Event<EventInput>::TypeIdClass()));
		const auto& InputEvent = static_cast<const Event<EventInput>&>(event);
		auto ID = m_ParentPlayer->GetPlayerID();
		if (InputEvent.Message().GetPlayerID() == ID)
		{
			const auto& PlayerInputs = InputEvent.Message().GetInputs();

			//break out the player translation and rotation inputs from the struct
			m_CurrentPlayerMove = PlayerInputs.LS * m_MovementSpeed;
			float tmp = InputEvent.Message().m_WorldState->GetGameTime().ElapsedGameTime().count() / 1000.f;
			m_CurrentPlayerMove *= tmp;
			auto& RightStick = PlayerInputs.RS;
			auto RightStickMagnitude = PlayerInputs.RSMagnitude;
			RightStickMagnitude; //TODO: remove stub;

			//If the player is the gun player, we need the Right Trigger value.
			const auto PlayerID = InputEvent.Message().GetPlayerID();
			auto RightTrigger = PlayerInputs.RT;

			if (RightStick != glm::vec4(0.f))
			{
				glm::vec4 turnDir = glm::vec4(0, 0, -RightStick.x + RightStick.z, 0);
				m_CurrentForwardVector = turnDir * m_MovementSpeed;
				m_ParentPlayer->SetRotation(m_CurrentForwardVector + m_ParentPlayer->GetRotation());
			}

			//move the player the specified amount
			auto& PlayerTransform = m_ParentPlayer->GetTransform();
			auto PlayerPos = PlayerTransform.GetPosition();
			Transform RotTransform(glm::vec4(0.f), PlayerTransform.GetRotation(), glm::vec4(1.f));
			auto RotatedVector = RotTransform * m_CurrentPlayerMove;
			m_CurrentPlayerMove = RotatedVector + m_ParentPlayer->GetPosition();
			if ((PlayerPos.x <= 9.5f && PlayerPos.x >= -9.5f))
			{
				if ((PlayerPos.y <= 5.75f && PlayerPos.y >= -5.75f))
				{
					m_ParentPlayer->SetPosition(m_CurrentPlayerMove);
				}
				else
				{
					if (PlayerPos.y > 5.75f)
					{
						PlayerPos.y = 5.74f;
						m_ParentPlayer->SetPosition(PlayerPos);
					}
					else if (PlayerPos.y < -5.75f)
					{
						PlayerPos.y = -5.74f;
						m_ParentPlayer->SetPosition(PlayerPos);
					}
				}
			}
			else
			{
				if (PlayerPos.x > 9.5f)
				{
					PlayerPos.x = 9.49f;
					m_ParentPlayer->SetPosition(PlayerPos);
				}
				else if (PlayerPos.x < -9.5f)
				{
					PlayerPos.x = -9.49f;
					m_ParentPlayer->SetPosition(PlayerPos);
				}
			}

			if (PlayerID == 0)
			{
				if (RightTrigger == 1.f)
				{
					int cooldown = m_ParentPlayer->GetCooldown();
					auto now = InputEvent.Message().m_WorldState->GetGameTime().CurrentTime();
					int duration = int((now - m_LastShootTime).count() / 1000000);
					if (duration > cooldown)
					{
						m_LastShootTime = now;
						Transform rot(glm::vec4(0), m_ParentPlayer->GetTransform().GetRotation(), glm::vec4(1));
						Bullet& bullet = *Bullet::CreateDefaultBullet();
						bullet.SetPosition(m_ParentPlayer->GetPosition() + rot * glm::vec4(0, 1, 0, 1));
						bullet["Velocity"] = rot * glm::vec4(0, 4, 0, 1);
						auto[datum, scope] = m_ParentPlayer->Search(Sector::ENTITY_TABLE_KEY);
						assert(scope->Is(Sector::TypeIdClass()));
						scope->Adopt(bullet, Sector::ENTITY_TABLE_KEY);
					}
				}
			}
		}

		//TODO: Pass player movement and new forward vector to collision and rotation systems
	}

	const Vector<Attributed::Signature> PlayerMovement::Signatures()
	{
		return Vector<Attributed::Signature>();
	}

	void PlayerMovement::SetParentPlayer(Player* ParentPlayer)
	{
		m_ParentPlayer = ParentPlayer;
	}

}