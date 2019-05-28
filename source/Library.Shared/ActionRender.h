#pragma once
#include "Action.h"
#include "Transform.h"

namespace GameEngine
{
	class ActionRender : public Action
	{
		RTTI_DECLARATIONS(ActionRender, Action);

	public:
		struct RenderParam
		{
			int mTextureId = 0;
			int mSpriteRow = 0;
			int mSpriteColumn = 0;
			bool mHorizontalFlip = false;
			bool mVerticalFlip = false;
			Transform mOffset;
		};

		ActionRender();
		virtual void Update(WorldState& state) override;
		virtual gsl::owner<ActionRender*> Clone() const override;
		const RenderParam& GetRenderParam() const;
		void SetTexture(int id);
		void SetSprite(int row, int column);
		void SetHorizontalFlip(bool flip);
		void SetVerticalFlip(bool flip);
		void SetOffset(const Transform& trans);
		void SetRenderParam(const RenderParam& param);
		static const Vector<Attributed::Signature> Signatures();

	protected:
		ActionRender(RTTI::IdType type);
		RenderParam mParam;
	};

	DECLARE_FACTORY(ActionRender, Scope);
}