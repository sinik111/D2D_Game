#pragma once

#include <string>
#include <unordered_map>

struct Sprite
{
	std::wstring name;
	float x;
	float y;
	float width;
	float height;
	float pivotX;
	float pivotY;
};

struct SpriteSheet
{
	std::wstring name;
	std::vector<Sprite> sprites;
	std::unordered_map<std::wstring, size_t> spriteIndexMap;
	float width;
	float height;
};