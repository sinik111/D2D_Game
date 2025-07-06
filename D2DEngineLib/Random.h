#pragma once

#include <random>
#include "Vector2.h"

class Random
{
public:
	static int Int(int min, int max);

	static float Float(float min, float max);

	// 0.0f ~ 1.0f -> 0% ~ 100%
	static bool Chance(float probability);

	static Vector2 InsideCircle();

	static Vector2 Direction(float minDegree = 0.0f, float maxDegree = 360.0f);
};