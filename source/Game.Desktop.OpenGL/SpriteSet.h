#pragma once
#include "vector.h"
#include "Sprite.h"

namespace GameEngine
{

	class SpriteSet
	{
	public:
		Vector<std::shared_ptr<Sprite>> mSpriteList;
		float mTimeBetweenFrames = 0;
		size_t currentFrame;

		SpriteSet();
		~SpriteSet();
		SpriteSet(int newSetSize, ...);

		void Add(std::shared_ptr<Sprite> newSprite);

		void ProgressFrame();

	private:
		float currentElapsedTime = 0;
	};

}