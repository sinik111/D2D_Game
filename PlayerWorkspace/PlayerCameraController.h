#pragma once

#include "../D2DEngineLib/Script.h"

enum class CameraShakeType
{
	LeftRight,
	UpDown,
	Random
};

class PlayerCameraController :
	public Script
{
private:
	Transform* m_target;
	Vector2 m_offset;

	std::vector<Vector2> m_randomShakeVector;
	CameraShakeType m_shakeType;
	Vector2 m_shakeVector;
	Vector2 m_lastShakeVector;
	float m_shakePower = 0.0f;
	float m_shakeDuration = 0.0f;
	float m_shakeTimer = 0.0f;
	int m_shakeCount = 0;
	bool m_isShaking = false;
	bool m_isDecreasingPower = false;

private:
	void Start() override;
	void Update() override;

public:
	void SetTarget(Transform* target);
	void SetOffset(const Vector2& offset);
	void ShakeCamera(CameraShakeType type, float power, float duration, int shakeCount, bool decreasingPower = false);
};