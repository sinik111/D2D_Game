#pragma once

#include "Component.h"
#include "AnimationData.h"
#include "Delegate.h"
#include "SpriteData.h"
#include "BitmapResource.h"

class BitmapRenderer;

class Animator :
	public Component
{
private:
	BitmapRenderer* m_bitmapRenderer = nullptr;
	AnimationClip* m_currentClip = nullptr;

	std::unordered_map<std::wstring, std::shared_ptr<SpriteSheet>> m_spriteSheets;
	std::unordered_map<std::wstring, std::shared_ptr<AnimationClip>> m_animationClips;
	std::unordered_map<std::wstring, std::shared_ptr<BitmapResource>> m_bitmapResources;

	std::unordered_map<std::wstring, Delegate<>> m_eventDelegates;

	float m_timer = 0.0f;
	float m_playSpeed = 1.0f;
	size_t m_frameCounter = 0;
	size_t m_eventCounter = 0;
	bool m_isFinished = true;

public:
	void Initialize() override;
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

public:
	void AddSpriteSheet(const std::wstring& filePath);
	void AddAnimationClip(const std::wstring& filePath);

	template<typename T>
	void AddActionOnEvent(const std::wstring& eventName, T* instance, void (T::* func)())
	{
		m_eventDelegates[eventName].Add(instance, func);
	}

	void Play(const std::wstring& clipName, size_t startFrame = 0);
	const std::wstring& GetCurrentClipName() const;
	bool IsFinished() const;
	void SetPlaySpeed(float playSpeed); // πË¿≤
	size_t GetCurrentFrame() const;

private:
	void Update();
	void SetSpriteData();

	friend class AnimatorSystem;
};