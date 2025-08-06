#include "../D2DEngineLib/framework.h"
#include "PlayerCameraController.h"

void PlayerCameraController::Start()
{
	GetTransform()->SetLocalPosition(m_target->GetLocalPosition() + m_offset);
}

void PlayerCameraController::Update()
{
    if (m_isShaking)
    {
        m_shakeTimer += MyTime::DeltaTime();
        if (m_shakeTimer >= m_shakeDuration)
        {
            m_shakeVector = { 0.0f, 0.0f };
            m_isShaking = false;
            m_shakeTimer = 0.0f;
            return;
        }

        float progress = m_shakeTimer / m_shakeDuration;
        float currentPower = m_isDecreasingPower ? m_shakePower * (1.0f - progress) : m_shakePower;

        switch (m_shakeType)
        {
        case CameraShakeType::LeftRight:
        {
            float shakeValue = std::sin(m_shakeTimer * m_shakeCount * MyMath::PI * 2.0f);
            m_shakeVector = { shakeValue, 0.0f };
            break;
        }
        case CameraShakeType::UpDown:
        {
            float shakeValue = std::sin(m_shakeTimer * m_shakeCount * MyMath::PI * 2.0f);
            m_shakeVector = { 0.0f, shakeValue };
            break;
        }
        case CameraShakeType::Random:
        {
            float shakeCycle = m_shakeDuration / m_shakeCount;
            int currentIndex = (int)(m_shakeTimer / shakeCycle);
            int nextIndex = currentIndex + 1;

            float cycleProgress = std::fmodf(m_shakeTimer, shakeCycle) / shakeCycle;

            Vector2 currentVector = m_randomShakeVector[currentIndex];
            Vector2 nextVector = nextIndex > m_randomShakeVector.size() - 1 ? Vector2(0.0f, 0.0f) : m_randomShakeVector[nextIndex];

            m_shakeVector = (1.0f - cycleProgress) * currentVector + cycleProgress * nextVector;
            break;
        }
        }

        m_shakeVector *= currentPower;

        GetTransform()->SetLocalPosition(m_target->GetLocalPosition() + m_offset + m_shakeVector);
    }
    else
    {
        GetTransform()->SetLocalPosition(m_target->GetLocalPosition() + m_offset);
    }
}

void PlayerCameraController::SetTarget(Transform* target)
{
	m_target = target;
}

void PlayerCameraController::SetOffset(const Vector2& offset)
{
	m_offset = offset;
}

void PlayerCameraController::ShakeCamera(CameraShakeType type, float power, float duration, int shakeCount, bool decreasingPower)
{
    m_shakeTimer = 0.0f;
	m_shakeType = type;
	m_shakePower = power;
	m_shakeDuration = duration;
	m_shakeCount = shakeCount;
	m_isShaking = true;
	m_isDecreasingPower = decreasingPower;

	if (!m_randomShakeVector.empty())
	{
		m_randomShakeVector.clear();
	}

	if (type == CameraShakeType::Random)
	{
		for (int i = 0; i < shakeCount; ++i)
		{
			m_randomShakeVector.push_back(Random::Direction());
		}
	}
}