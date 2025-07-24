#include "pch.h"
#include "Particle.h"

#include "ComponentSystem.h"
#include "GameObject.h"

void Particle::RegisterToSystem()
{
	ComponentSystem::Get().Particle().RegisterParticle(this);
}

void Particle::UnregisterFromSystem()
{
	ComponentSystem::Get().Particle().UnregisterParticle(this);
}

void Particle::Initialize()
{
	m_batchRenderer = GetGameObject()->GetComponent<BatchRenderer>();

	assert(m_batchRenderer != nullptr && L"Particle 컴포넌트가 참조할 BatchRenderer가 없습니다");

	m_batchRenderer->SetBitmap(m_bitmapPath);
	m_batchRenderer->SetSpriteSheet(m_sheetPath);
}

void Particle::Update()
{
	if (!m_isFinished)
	{
		const float deltaTime = MyTime::DeltaTime();

		m_batchRenderer->ClearBatchUnitList();

		m_timer += deltaTime;

		if (m_timer >= m_duration)
		{
			if (!m_isLoop)
			{
				m_isFinished = true;

				return;
			}

			m_timer -= m_duration;
		}

		for (auto& particleUnit : m_particleUnits)
		{
			if (!particleUnit.isPlaying)
			{
				if (particleUnit.startTime <= m_timer)
				{
					particleUnit.isPlaying = true;

					if (!particleUnit.isFollowing)
					{
						particleUnit.original = GetTransform()->GetWorldMatrix();
					}
				}
			}
			
			if (particleUnit.isPlaying)
			{
				particleUnit.timer += deltaTime;

				if (particleUnit.timer >= particleUnit.duration)
				{
					particleUnit.timer -= particleUnit.duration;

					particleUnit.isPlaying = false;

					continue;
				}

				size_t size = particleUnit.batchUnits.size();
				
				if (!particleUnit.doInterpolate)
				{
					size_t index = static_cast<size_t>((particleUnit.timer / particleUnit.duration) * size);

					BatchUnit batchUnit = particleUnit.batchUnits[index];
					if (!particleUnit.isFollowing)
					{
						batchUnit.isFollowing = false;
						batchUnit.world = particleUnit.original;
					}

					m_batchRenderer->AddBatchUnit(batchUnit);
				}
				else
				{
					size_t index = static_cast<size_t>((particleUnit.timer / particleUnit.duration) * (size - 1));

					BatchUnit batchUnit = BatchUnit::Lerp(
						particleUnit.batchUnits[index],
						particleUnit.batchUnits[index + 1],
						(particleUnit.timer / particleUnit.duration) * (size - 1) - index
					);
					batchUnit.index = particleUnit.batchUnits[index].index;

					if (!particleUnit.isFollowing)
					{
						batchUnit.isFollowing = false;
						batchUnit.world = particleUnit.original;
					}

					m_batchRenderer->AddBatchUnit(batchUnit);
				}
			}
		}
	}
}

float Particle::GetDuration() const
{
	return m_duration;
}

bool Particle::GetLoop() const
{
	return m_isLoop;
}

bool Particle::IsFinished() const
{
	return m_isFinished;
}

void Particle::SetDuration(float duration)
{
	m_duration = duration;
}

void Particle::SetLoop(bool isLoop)
{
	m_isLoop = isLoop;
}

void Particle::SetBitmap(const std::wstring& filePath)
{
	m_bitmapPath = filePath;
}

void Particle::SetSpriteSheet(const std::wstring& filePath)
{
	m_sheetPath = filePath;
}

void Particle::AddParticleUnit(const ParticleUnit& particleUnit)
{
	m_particleUnits.push_back(particleUnit);
}