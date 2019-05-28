#include "pch.h"
#include "ActionRender.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionRender);

ActionRender::ActionRender() :
	ActionRender(ActionRender::TypeIdClass())
{}

ActionRender::ActionRender(RTTI::IdType type) :
	Action(type)
{}

void ActionRender::Update(WorldState& state)
{
	Action::Update(state);
}

gsl::owner<ActionRender*> ActionRender::Clone() const
{
	return new ActionRender(*this);
}

const ActionRender::RenderParam& ActionRender::GetRenderParam() const
{
	Entity& entity = dynamic_cast<Entity&>(*GetParent());
	Datum& actions = entity.Actions();
	for (size_t i = 0; i < actions.Size(); ++i)
	{
		Action& scope = static_cast<Action&>(actions[int(i)]);
		scope.Size();
	}
	return mParam;
}

void ActionRender::SetTexture(int id)
{
	mParam.mTextureId = id;
}

void ActionRender::SetSprite(int row, int column)
{
	mParam.mSpriteRow = row;
	mParam.mSpriteColumn = column;
}

void ActionRender::SetHorizontalFlip(bool flip)
{
	mParam.mHorizontalFlip = flip;
}

void ActionRender::SetVerticalFlip(bool flip)
{
	mParam.mVerticalFlip = flip;
}

void ActionRender::SetOffset(const Transform& trans)
{
	mParam.mOffset = trans;
}

void ActionRender::SetRenderParam(const RenderParam& param)
{
	mParam = param;
}

const Vector<Attributed::Signature> ActionRender::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Texture", Datum::DatumType::Integer, 1, offsetof(ActionRender, mParam.mTextureId)),
		Signature("Row", Datum::DatumType::Integer, 1, offsetof(ActionRender, mParam.mSpriteRow)),
		Signature("Column", Datum::DatumType::Integer, 1, offsetof(ActionRender, mParam.mSpriteColumn)),
		Signature("HorizontalFlip", Datum::DatumType::Integer, 1, offsetof(ActionRender, mParam.mHorizontalFlip)),
		Signature("VerticalFlip", Datum::DatumType::Integer, 1, offsetof(ActionRender, mParam.mVerticalFlip))
	});
}