#pragma once

#include "Component.h"
#include "AnimationData.h"
#include "Delegate.h"

class BitmapRenderer;

class Animator :
	public Component
{
private:
	BitmapRenderer* m_bitmapRenderer = nullptr;
	AnimationClip* m_currentClip = nullptr;

	std::shared_ptr<SpriteSheet> m_spriteSheet;
	std::unordered_map<std::wstring, std::shared_ptr<AnimationClip>> m_animationClips;

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
	void SetSpriteSheet(const std::wstring& filePath);
	void AddAnimationClip(const std::wstring& filePath);

	template<typename T>
	void AddActionOnEvent(const std::wstring& eventName, T* instance, void (T::* func)())
	{
		m_eventDelegates[eventName].Add(instance, func);
	}

	void Play(const std::wstring& clipName);
	const std::wstring& GetCurrentClipName() const;
	bool IsFinished() const;
	void SetPlaySpeed(float playSpeed); // ����

private:
	void Update();
	void SetSpriteData();

	friend class AnimatorSystem;
};