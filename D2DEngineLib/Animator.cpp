#include "pch.h"
#include "Animator.h"

#include "GameObject.h"
#include "BitmapRenderer.h"
#include "MyTimeSystem.h"
#include "ResourceManager.h"
#include "ComponentSystem.h"
#include "Debug.h"

Animator::Animator()
{
	ComponentSystem::Get().Animator().Register(this);
}

Animator::~Animator()
{
	ComponentSystem::Get().Animator().Unregister(this);
}

void Animator::SetSpriteSheet(const std::wstring& filePath)
{
	m_spriteSheet = ResourceManager::Get().CreateSpriteSheet(filePath);
}

void Animator::AddAnimationClip(const std::wstring& clipName, const std::wstring& filePath)
{
	if (m_animationClips.find(clipName) != m_animationClips.end())
	{
		assert(false && L"이미 있는 클립 이름");

		return;
	}

	m_animationClips[clipName] = ResourceManager::Get().CreateAnimationClip(filePath, m_spriteSheet);
}

void Animator::Play(const std::wstring& clipName)
{
	if (m_bitmapRenderer == nullptr)
	{
		m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();

		assert(m_bitmapRenderer != nullptr && L"Animator가 참조할 BitmapRenderer가 없습니다");
	}

	const auto& iter = m_animationClips.find(clipName);
	if (iter == m_animationClips.end())
	{
		assert(false && L"없는 clipName");

		return;
	}

	m_currentClip = iter->second.get();
	m_currentClipName = clipName;

	m_bitmapRenderer->SetBitmap(m_currentClip->filePath);

	m_timer = 0.0f;
	m_frameCounter = 0;
	m_eventCounter = 0;

	SetSpriteData();

	// 0 sec event call

	m_isFinished = false;
}

const std::wstring& Animator::GetCurrentClipName() const
{
	return m_currentClipName;
}

bool Animator::IsFinished() const
{
	return m_isFinished;
}

void Animator::Update()
{
	if (!m_isFinished)
	{
		m_timer += MyTimeSystem::Get().DeltaTime();

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

		if (m_eventCounter < m_currentClip->events.size())
		{
			if (m_timer >= m_currentClip->frames[m_eventCounter].time)
			{
				++m_eventCounter;

				if (m_eventCounter < m_currentClip->events.size())
				{
					// event call
				}
			}
		}
	}
}

void Animator::SetSpriteData()
{
	const Sprite& sprite = m_spriteSheet->
		sprites[m_currentClip->frames[m_frameCounter % m_currentClip->frames.size()].spriteIndex];

	// rect, pivot 설정 및 window -> unity 좌표계 변경으로 인한 좌표 조정
	m_bitmapRenderer->SetSourceRect({
			sprite.x,
			m_spriteSheet->height - sprite.y,
			sprite.x + sprite.width,
			m_spriteSheet->height - sprite.y - sprite.height
		}
	);

	m_bitmapRenderer->SetPivot({ sprite.pivotX, sprite.pivotY - 1.0f });
}
