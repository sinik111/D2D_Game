#pragma once

#include "../D2DEngineLib/Script.h"

class Transform;

class FollowCameraController :
	public Script
{
private:
	Transform* m_target;

	Vector2 m_offset{};

public:
	void SetTarget(Transform* target);

private:
	void Start() override;
	void Update() override;
};

