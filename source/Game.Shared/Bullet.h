#pragma once
#include "Entity.h"
#include <glm/fwd.hpp>

namespace GameEngine
{
	class Bullet final : public Entity
	{
		RTTI_DECLARATIONS(Bullet, Entity);

	public:
		Bullet();
		virtual void Update(WorldState& state) override;
		virtual gsl::owner<Bullet*> Clone() const override;
		static const Vector<Attributed::Signature> Signatures();
		static gsl::owner<Bullet*> CreateDefaultBullet();

	private:
		glm::vec4 mVelocity = glm::vec4(0);
	};

	DECLARE_FACTORY(Bullet, Scope);
}
