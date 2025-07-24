#pragma once

#include "Component.h"
#include "BatchRenderer.h"

struct ParticleUnit
{
	std::vector<BatchUnit> batchUnits;
	float duration = 0.0f;
	float startTime = 0.0f;
	float timer = 0.0f;
	Matrix3x2 original;
	bool isPlaying = false;
	bool doInterpolate = true;
	bool isFollowing = true;
};

class Particle :
	public Component
{
private:
	BatchRenderer* m_batchRenderer = nullptr;
	std::vector<ParticleUnit> m_particleUnits;
	std::wstring m_bitmapPath;
	std::wstring m_sheetPath;
	float m_duration = 0.0f;
	float m_timer = 0.0f;
	bool m_isLoop = false;
	bool m_isFinished = false;

public:
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

public:
	void Initialize() override;
	void Update();

public:
	float GetDuration() const;
	bool GetLoop() const;
	bool IsFinished() const;

	void SetDuration(float duration);
	void SetLoop(bool isLoop);
	void SetBitmap(const std::wstring& filePath);
	void SetSpriteSheet(const std::wstring& filePath);

	void AddParticleUnit(const ParticleUnit& particleUnit);
};