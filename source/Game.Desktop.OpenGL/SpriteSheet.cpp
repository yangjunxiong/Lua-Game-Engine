#include "pch.h"
#include "SpriteSheet.h"
#include <iostream>
#include <fstream>
#include "Transform.h"
#include <GL/gl3w.h>

using namespace std;
using namespace glm;

namespace GameEngine
{

	SpriteSheet::SpriteSheet(const char* fileName, size_t numSpritesW, size_t numSpritesH, bool linear) :
		mSpriteSheetWidth(numSpritesW), mSpriteSheetHeight(numSpritesH)
	{
		mFileName = fileName;
		if (fileName == nullptr || strcmp(mFileName, "") == 0) throw std::exception("Image file name must be specified");

		glGenTextures(1, &boundTexture);

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, boundTexture);


		unsigned char* mImage = SOIL_load_image(mFileName, &mTextureWidth, &mTextureHeight, 0, SOIL_LOAD_AUTO);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImage);
		SOIL_free_image_data(mImage);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

		if (linear)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}


	}


	SpriteSheet::~SpriteSheet()
	{
		glDeleteTextures(1, &boundTexture);
	}

	std::shared_ptr<Sprite> SpriteSheet::GetSprite(size_t spriteColumn, size_t spriteRow)
	{
		spriteColumn;
		spriteRow;
		float spriteWidth = 1.f / float(mSpriteSheetWidth);
		float spriteHeight = 1.f / float(mSpriteSheetHeight);
		size_t column = spriteColumn % mSpriteSheetWidth;
		size_t row = spriteRow % mSpriteSheetHeight;

		GLfloat newVertBuffer[] = {
			//  Position            Color                 Texcoords
				-0.5f,  0.5f, 0.f, 1.0f, 1.0f, 1.0f, 1.f, (float(column) * spriteWidth), (float(row) * spriteHeight), // Top-left
				 0.5f,  0.5f, 0.f, 1.0f, 1.0f, 1.0f, 1.f, ((float(column) + 1.f) * spriteWidth), (float(row) * spriteHeight), // Top-right
				 0.5f, -0.5f, 0.f, 1.0f, 1.0f, 1.0f, 1.f, ((float(column) + 1.f) * spriteWidth), ((float(row) + 1.f) * spriteHeight), // Bottom-right
				-0.5f, -0.5f, 0.f, 1.0f, 1.0f, 1.0f, 1.f, (float(column) * spriteWidth), ((float(row) + 1.f) * spriteHeight)  // Bottom-left
		};
		return std::make_shared<Sprite>(this, newVertBuffer);
	}

}