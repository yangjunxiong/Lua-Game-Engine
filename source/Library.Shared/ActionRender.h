#pragma once
#include "Action.h"

namespace GameEngine
{
	CLASS(NoLuaAuthority);
	class ActionRender : public Action
	{
		RTTI_DECLARATIONS(ActionRender, Action);

	public:
		ActionRender(Entity* parent);
		virtual void Draw() = 0;
		virtual void Update(WorldState&) {};
		virtual gsl::owner<Scope*> Clone() const override { return nullptr; };
		static const Vector<Attributed::Signature> Signatures();

		PROPERTY();
		bool Visible = true;

	protected:
		ActionRender(RTTI::IdType type, Entity* parent);
	};
}