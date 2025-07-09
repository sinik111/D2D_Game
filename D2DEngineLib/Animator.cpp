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

void Animator::AddAnimationClip(const std::wstring& filePath)
{
	std::shared_ptr<AnimationClip> clip = ResourceManager::Get().CreateAnimationClip(filePath, m_spriteSheet);

	m_animationClips[clip->name] = clip;
}

void Animator::Play(const std::wstring& clipName)
{
	if (m_bitmapRenderer == nullptr)
	{
		m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();

		assert(m_bitmapRenderer != nullptr && L"Animator�� ������ BitmapRenderer�� �����ϴ�");
	}

	const auto& iter = m_animationClips.find(clipName);
	if (iter == m_animationClips.end())
	{
		assert(false && L"���� clipName");

		return;
	}

	m_currentClip = iter->second.get();

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

void Animator::Update()
{
	if (!m_isFinished)
	{
		m_timer += MyTimeSystem::Get().GetDeltaTime() * m_playSpeed;

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
	// Ȥ�� �� �ε��� �ʰ��� ����ؼ� % �����ڷ� ������
	const Sprite& sprite = m_spriteSheet->
		sprites[m_currentClip->frames[m_frameCounter % m_currentClip->frames.size()].spriteIndex];

	// rect, pivot ���� �� window -> unity ��ǥ�� �������� ���� ��ǥ ����
	m_bitmapRenderer->SetSourceRect({
			sprite.x,
			m_spriteSheet->height - sprite.y,
			sprite.x + sprite.width,
			m_spriteSheet->height - sprite.y - sprite.height
		}
	);

	m_bitmapRenderer->SetPivot({ sprite.pivotX, sprite.pivotY });
}
