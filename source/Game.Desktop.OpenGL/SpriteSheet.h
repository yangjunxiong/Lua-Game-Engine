#pragma once
#include <string>
#include "Sprite.h"
#include <memory>

namespace GameEngine
{
	class Sprite;

	class SpriteSheet
	{
	public:
		const size_t mSpriteSheetWidth;
		const size_t mSpriteSheetHeight;
		int mTextureWidth;
		int mTextureHeight;
		const char* mFileName;
		unsigned int boundTexture;

		SpriteSheet(const char* fileName, size_t numSpritesW = 1, size_t numSpritesH = 1, bool linear = true);
		~SpriteSheet();

		/// <summary>
		/// Returns a new Sprite object from the position in the spriteSheet.
		/// </summary>
		/// <param name="spriteColumn">Which column to take the sprite from.</param>
		/// <param name="spriteRow">Which row to take the sprite from.</param>
		/// <returns>Sprite pointer to newly created object.</returns>
		std::shared_ptr<Sprite> GetSprite(size_t spriteColumn, size_t spriteRow);
	};

}