#pragma once
#include "Action.h"
#include "Transform.h"

namespace GameEngine
{
	CLASS();
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

		FUNCTION();
		virtual void Update(WorldState& state) override;
		virtual gsl::owner<ActionRender*> Clone() const override;
		const RenderParam& GetRenderParam() const;

		FUNCTION();
		void SetTexture(int id);

		FUNCTION();
		void SetSprite(int row, int column);

		FUNCTION();
		void SetHorizontalFlip(bool flip);

		FUNCTION();
		void SetVerticalFlip(bool flip);

		FUNCTION();
		void SetOffset(const Transform& trans);
		void SetRenderParam(const RenderParam& param);
		static const Vector<Attributed::Signature> Signatures();

	protected:
		ActionRender(RTTI::IdType type);
		RenderParam mParam;
	};

	DECLARE_FACTORY(ActionRender, Scope);
}