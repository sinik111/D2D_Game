#include "pch.h"
#include "Random.h"

#include "MyMath.h"

static std::mt19937 gen{ std::random_device{}() };

int Random::Int(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(gen);
}

float Random::Float(float min, float max)
{
	return std::uniform_real_distribution<float>(min, max)(gen);
}

bool Random::Chance(float probability)
{
	assert((probability >= 0.0f && probability <= 1.0f) && "Ȯ���� 0 ~ 1 ���̷� ���ּ���");

	return std::uniform_real_distribution<float>(0.0f, 1.0f)(gen) <= probability;
}

Vector2 Random::InsideCircle()
{
	Vector2 result;

	while (true)
	{
		result.x = std::uniform_real_distribution<float>(-1.0f, 1.0f)(gen);
		result.y = std::uniform_real_distribution<float>(-1.0f, 1.0f)(gen);

		if (result.LengthSq() <= 1.0f)
		{
			break;
		}
	}

	return result;
}

Vector2 Random::Direction(float minDegree, float maxDegree)
{
	float degree = std::uniform_real_distribution<float>(minDegree, maxDegree)(gen);
	float radian = degree * MyMath::DegToRad;

	return Vector2(std::cos(radian), std::sin(radian));
}