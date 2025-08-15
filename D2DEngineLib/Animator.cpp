#include "pch.h"
#include "Animator.h"

#include "GameObject.h"
#include "BitmapRenderer.h"
#include "MyTimeSystem.h"
#include "ResourceManager.h"
#include "ComponentSystem.h"
#include "Debug.h"

void Animator::Initialize()
{
	m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();

	assert(m_bitmapRenderer != nullptr && L"Animator가 참조할 BitmapRenderer가 없습니다");
}

void Animator::RegisterToSystem()
{
	ComponentSystem::Get().Animator().Register(this);
}

void Animator::UnregisterFromSystem()
{
	ComponentSystem::Get().Animator().Unregister(this);
}

void Animator::AddSpriteSheet(const std::wstring& filePath)
{
	auto sheet = ResourceManager::Get().CreateSpriteSheet(filePath);

	m_spriteSheets[sheet->name] = sheet;

	m_bitmapResources[sheet->name] = ResourceManager::Get().CreateBitmapResource(sheet->name);
}

void Animator::AddAnimationClip(const std::wstring& filePath)
{
	std::shared_ptr<AnimationClip> clip = ResourceManager::Get().CreateAnimationClip(filePath, m_spriteSheets);

	m_animationClips[clip->name] = clip;
}

void Animator::Play(const std::wstring& clipName, size_t startFrame)
{
	const auto& iter = m_animationClips.find(clipName);
	if (iter == m_animationClips.end())
	{
		assert(false && L"없는 clipName");

		return;
	}

	m_currentClip = iter->second.get();

	m_bitmapRenderer->SetBitmap(m_bitmapResources[m_currentClip->filePath]);

	if (startFrame > m_currentClip->frames.size() - 1)
	{
		startFrame = 0;
	}

	m_timer = m_currentClip->frames[startFrame].time;
	m_frameCounter = startFrame;

	size_t eventCount = 0;
	for (auto& event : m_currentClip->events)
	{
		if (event.time <= m_timer)
		{
			++eventCount;
		}
	}

	m_eventCounter = eventCount;

	SetSpriteData();

	if (!m_currentClip->events.empty() && m_currentClip->events[0].time <= 0.0f)
	{
		const auto& iter = m_eventDelegates.find(m_currentClip->events[0].name);
		if (iter != m_eventDelegates.end())
		{
			iter->second.Invoke();
		}

		++m_eventCounter;
	}

	m_isFinished = false;
}

const std::wstring& Animator::GetCurrentClipName() const
{
	return m_currentClip->name;
}

bool Animator::IsFinished() const
{
	return m_isFinished;
}

void Animator::SetPlaySpeed(float playSpeed)
{
	m_playSpeed = playSpeed;
}

size_t Animator::GetCurrentFrame() const
{
	return m_frameCounter;
}

void Animator::Update()
{
	if (!m_isFinished)
	{
		m_timer += MyTimeSystem::Get().DeltaTime() * m_playSpeed;

		if (m_timer >= m_currentClip->duration)
		{
			if (m_currentClip->isLoop)
			{
				m_timer -= m_currentClip->duration;
				m_frameCounter = 0;
				m_eventCounter = 0;

				SetSpriteData();
			}
			else
			{
				m_isFinished = true;

				return;
			}
		}

		float nextFrameTime;
		if (m_frameCounter < m_currentClip->frames.size() - 1)
		{
			nextFrameTime = m_currentClip->frames[m_frameCounter + 1].time;
		}
		else
		{
			nextFrameTime = m_currentClip->duration;
		}

		if (m_timer >= nextFrameTime)
		{
			++m_frameCounter;

			SetSpriteData();
		}

		if (m_currentClip->events.size() > m_eventCounter)
		{
			const auto& event = m_currentClip->events[m_eventCounter];
			if (m_timer >= event.time)
			{
				const auto& iter = m_eventDelegates.find(event.name);
				if (iter != m_eventDelegates.end())
				{
					iter->second.Invoke();
				}

				++m_eventCounter;
			}
		}
	}
}

void Animator::SetSpriteData()
{
	// 혹시 모를 인덱스 초과를 대비해서 % 연산자로 나눠줌
	const Sprite& sprite = m_spriteSheets[m_currentClip->filePath]->
		sprites[m_currentClip->frames[m_frameCounter % m_currentClip->frames.size()].spriteIndex];

	// rect, pivot 설정 및 window -> unity 좌표계 변경으로 인한 좌표 조정
	m_bitmapRenderer->SetSourceRect({
			sprite.x,
			sprite.y,
			sprite.x + sprite.width,
			sprite.y + sprite.height
		}
	);

	m_bitmapRenderer->SetPivot({ sprite.pivotX, sprite.pivotY });
}
