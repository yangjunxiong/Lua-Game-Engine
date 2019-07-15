#include "pch.h"
#include "Vector4.h"

using namespace GameEngine;

Vector4::Vector4() :
	mVector(0.f, 0.f, 0.f, 0.f)
{}

Vector4::Vector4(float v) :
	mVector(v, v, v, v)
{}

Vector4::Vector4(float x, float y, float z, float w) :
	mVector(x, y, z, w)
{}

Vector4::Vector4(float x, float y, float z) :
	mVector(x, y, z, 1.f)
{}

Vector3::Vector3() :
	Vector4(0.f, 0.f, 0.f, 1.f)
{}

Vector3::Vector3(float v) :
	Vector4(v)
{}

Vector3::Vector3(float x, float y, float z) :
	Vector4(x, y, z, 1.f)
{}
