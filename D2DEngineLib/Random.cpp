#include "pch.h"
#include "Random.h"

static std::mt19937 gen(std::random_device{}());

int Random::Int(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(gen);
}

float Random::Float(float min, float max)
{
	return std::uniform_real_distribution<float>(min, max)(gen);
}