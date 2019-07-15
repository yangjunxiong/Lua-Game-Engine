#include "pch.h"
#include "Quaternion.h"

using namespace GameEngine;

const Quaternion Quaternion::Identity;

Quaternion::Quaternion() :
	mQuaternion(0.f, 0.f, 0.f, 1.f)
{}

Quaternion::Quaternion(float x, float y, float z, float w) :
	mQuaternion(x, y, z, w)
{}