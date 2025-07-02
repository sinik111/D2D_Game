#pragma once

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

struct FrameInfo
{
	std::wstring spriteName;
	size_t spriteIndex;
	float time;
};

struct EventInfo
{
	std::wstring name;
	std::wstring parameter;
	float time;
};

struct AnimationClip
{
	std::wstring name;
	std::wstring filePath;
	std::vector<FrameInfo> frames;
	std::vector<EventInfo> events;
	float duration;
	bool isLoop;
};

struct SpriteSheet
{
	std::wstring name;
	std::vector<Sprite> sprites;
	std::unordered_map<std::wstring, size_t> spriteIndexMap;
	float width;
	float height;
};