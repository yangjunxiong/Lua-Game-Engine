#include "pch.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace GameEngine;
using namespace std;
using namespace glm;

#define COMPARE_THRESHOLD 1e-5

Transform::Transform(const glm::vec4& position, const glm::vec4& rotation, const glm::vec4& scale) :
	mPosition(position),
	mRotation(rotation),
	mScale(scale)
{
	UpdateMatrix();
}

Transform::Transform(const glm::mat4& matrix) :
	mMatrix(matrix)
{
	UpdateTransform();
}

glm::vec4 Transform::operator*(const glm::vec4& point) const
{
	return mMatrix * point;
}

Transform Transform::operator*(const Transform& trans) const
{
	return Transform(mMatrix * trans.mMatrix);
}

void Transform::SetPosition(const glm::vec4& position)
{
	mPosition = position;
	UpdateMatrix();
}

void Transform::SetPosition(float x, float y, float z)
{
	SetPosition(vec4(x, y, z, 1));
}

void Transform::SetRotation(const glm::vec4& rotation)
{
	mRotation = rotation;
	UpdateMatrix();
}

void Transform::SetRotation(float x, float y, float z)
{
	SetRotation(vec4(x, y, z, 1));
}

void Transform::SetScale(const glm::vec4& scale)
{
	mScale = scale;
	UpdateMatrix();
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(vec4(x, y, z, 1));
}

const glm::vec4& Transform::GetPosition() const
{
	return mPosition;
}

const glm::vec4& Transform::GetRotation() const
{
	return mRotation;
}

const glm::vec4& Transform::GetScale() const
{
	return mScale;
}

const glm::mat4& Transform::GetMatrix() const
{
	return mMatrix;
}

bool Transform::Equal(float f1, float f2)
{
	return fabsf(f1 - f2) < COMPARE_THRESHOLD;
}

bool Transform::Equal(const glm::vec4& v1, const glm::vec4& v2)
{
	return fabsf(v1.x - v2.x) < COMPARE_THRESHOLD
		&& fabsf(v1.y - v2.y) < COMPARE_THRESHOLD
		&& fabsf(v1.z - v2.z) < COMPARE_THRESHOLD
		&& fabsf(v1.w - v2.w) < COMPARE_THRESHOLD;
}

// Notice that OpenGL matrix is column major, so we have to transpose from normally seen row major matrix
// Wasted an entire day on this trick
void Transform::UpdateMatrix()
{
	// Translation
	mat4 translate = mat4(1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  mPosition.x, mPosition.y, mPosition.z, 1);

	// X rotation
	float radianX = radians(mRotation.x);
	float sinX = sin(radianX);
	float cosX = cos(radianX);
	mat4 rotateX = mat4(1, 0, 0, 0,
						0, cosX, sinX, 0,
						0, -sinX, cosX, 0,
						0, 0, 0, 1);

	// Y rotation
	float radianY = radians(mRotation.y);
	float sinY = sin(radianY);
	float cosY = cos(radianY);
	mat4 rotateY = mat4(cosY, 0, -sinY, 0,
						0, 1, 0, 0,
						sinY, 0, cosY, 0,
						0, 0, 0, 1);

	// Z rotation
	float radianZ = radians(mRotation.z);
	float sinZ = sin(radianZ);
	float cosZ = cos(radianZ);
	mat4 rotateZ = mat4(cosZ, sinZ, 0, 0,
						-sinZ, cosZ, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);

	// Scale
	mat4 scale = mat4(mScale.x, 0, 0, 0,
					  0, mScale.y, 0, 0,
					  0, 0, mScale.z, 0,
					  0, 0, 0, 1);

	// Translate first -> then rotate xyz -> then scale
	mMatrix = translate * rotateX * rotateY * rotateZ * scale;
}

void Transform::UpdateTransform()
{
	// Tranlation
	mPosition = vec4(mMatrix[3][0], mMatrix[3][1], mMatrix[3][2], 1);

	// Scale
	float sx = length(vec3(mMatrix[0][0], mMatrix[0][1], mMatrix[0][2]));
	float sy = length(vec3(mMatrix[1][0], mMatrix[1][1], mMatrix[1][2]));
	float sz = length(vec3(mMatrix[2][0], mMatrix[2][1], mMatrix[2][2]));
	mScale = vec4(sx, sy, sz, 1);

	// Rotation
	mat4 rotM = mat4(mMatrix[0][0] / sx, mMatrix[0][1] / sx, mMatrix[0][2] / sx, 0,
					 mMatrix[1][0] / sy, mMatrix[1][1] / sy, mMatrix[1][2] / sy, 0,
					 mMatrix[2][0] / sz, mMatrix[2][1] / sz, mMatrix[2][2] / sz, 0,
					 0, 0, 0, 1);
	float x, y, z;
	if (rotM[2][0] != 1 && rotM[2][0] != -1)
	{
		y = asin(rotM[2][0]);
		float cy = cos(y);
		x = atan2(-rotM[2][1] / cy, rotM[2][2] / cy);
		z = atan2(-rotM[1][0] / cy, rotM[0][0] / cy);
	}
	else  // Can't solve this case for now
	{
		z = 0;
		if (rotM[2][0] == 1)
		{
			y = half_pi<float>();
			x = atan2(rotM[0][1], rotM[0][2]);
		}
		else
		{
			y = -half_pi<float>();
			x = atan2(-rotM[0][1], -rotM[0][2]);
		}
	}
	mRotation = vec4(degrees(x), degrees(y), degrees(z), 1);
}