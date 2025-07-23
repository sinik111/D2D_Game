#pragma once

#include <string>
#include <vector>

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
