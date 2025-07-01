#pragma once

#include "Component.h"
#include "AnimationData.h"

class BitmapRenderer;

class Animator :
	public Component
{
private:
	BitmapRenderer* m_bitmapRenderer = nullptr;
	AnimationClip* m_currentClip = nullptr;

	std::shared_ptr<SpriteSheet> m_spriteSheet;
	std::unordered_map<std::wstring, std::shared_ptr<AnimationClip>> m_animationClips;

	float m_timer = 0.0f;
	size_t m_frameCounter = 0;
	size_t m_eventCounter = 0;
	bool m_isFinished = true;

public:
	Animator();
	~Animator() override;

public:
	void SetSpriteSheet(const std::wstring& filePath);
	void AddAnimationClip(const std::wstring& clipName, const std::wstring& filePath);

	void Play(const std::wstring& clipName);
	bool IsFinished() const;

private:
	void Update();
	void SetSpriteData();

	friend class AnimatorSystem;
};