#pragma once
#include "SpriteSheet.h"
#include "Transform.h"
#include <SOIL/SOIL.h>

namespace GameEngine
{
	class SpriteSheet;

	class Sprite
	{
	public:
		const SpriteSheet* mSourceSheet;
		float vertexBuffer[36];
		
		Sprite(const SpriteSheet* sourceSheet, GLfloat newVertexBuffer[]);
		~Sprite();

		void Render(const Transform& trans) const;
	};
}

