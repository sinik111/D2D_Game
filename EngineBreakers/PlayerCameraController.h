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
	std::vector<GameObject*> m_mapObjects;
	CameraShakeType m_shakeType;
	Vector2 m_shakeVector;
	Vector2 m_lastShakeVector;
	Vector2 m_lerpStartPosition;
	Vector2 m_lerpEndPosition;

	float m_shakePower = 0.0f;
	float m_shakeDuration = 0.0f;
	float m_shakeTimer = 0.0f;
	int m_shakeCount = 0;
	float m_lerpTimer = 0.0f;
	float m_lerpDuration = 0.0f;
	bool m_isShaking = false;
	bool m_isDecreasingPower = false;
	bool m_isLerp = false;

private:
	void Start() override;
	void Update() override;
	void LateUpdate() override;

public:
	void SetTarget(Transform* target);
	void SetOffset(const Vector2& offset);
	void ShakeCamera(CameraShakeType type, float power, float duration, int shakeCount, bool decreasingPower = false);
	void SetMapObjects(std::vector<GameObject*> mapObjects);
	void StartLerp(const Vector2& startPosition, const Vector2& endPosition, float duration);
};