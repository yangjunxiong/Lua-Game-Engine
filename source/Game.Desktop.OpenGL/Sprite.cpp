#include "pch.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/gl3w.h>
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)
#include "Game.h"

using namespace glm;
using namespace std;

namespace GameEngine
{

	Sprite::Sprite(const SpriteSheet* sourceSheet, float newVertexBuffer[]) :
		mSourceSheet(sourceSheet)
	{
		for (int i = 0; i < 36; ++i)
		{
			vertexBuffer[i] = newVertexBuffer[i];
		}
	}

	Sprite::~Sprite()
	{
		
	}

	void Sprite::Render(const Transform& trans) const
	{
		// Use the texture that this sprite belongs to
		glBindTexture(GL_TEXTURE_2D, mSourceSheet->boundTexture);

		// Apply model transform matrix
		mat4 mat = trans.GetMatrix();
		GLint uniTrans = glGetUniformLocation(Game::GetInstance().mShaderProgram, "model");
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat));
		
		// Draw it !
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), &vertexBuffer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}