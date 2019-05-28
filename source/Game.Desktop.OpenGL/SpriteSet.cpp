#include "pch.h"
#include "SpriteSet.h"
#include <cstdarg>

namespace GameEngine
{

	SpriteSet::SpriteSet() :
		mSpriteList(Vector<std::shared_ptr<Sprite>>())
	{
	}


	SpriteSet::~SpriteSet()
	{
	}

	SpriteSet::SpriteSet(int newSetSize, ...) :
		mSpriteList(Vector<std::shared_ptr<Sprite>>(newSetSize))
	{
		va_list spritesToAdd;

		va_start(spritesToAdd, newSetSize);
		for (int i = 0; i < newSetSize; ++i)
		{
			mSpriteList.PushBack(va_arg(spritesToAdd, std::shared_ptr<Sprite>));
		}
		va_end(spritesToAdd);
	}

	void SpriteSet::Add(std::shared_ptr<Sprite> newSprite)
	{
		mSpriteList.PushBack(newSprite);
	}

	void SpriteSet::ProgressFrame()
	{
		++currentFrame;

		if (currentFrame >= mSpriteList.Size()) currentFrame = 0;
	}

}