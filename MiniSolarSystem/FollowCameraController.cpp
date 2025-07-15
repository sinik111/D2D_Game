#include "../D2DEngineLib/framework.h"
#include "FollowCameraController.h"

void FollowCameraController::SetTarget(Transform* target)
{
	m_target = target;
}

void FollowCameraController::Start()
{
	m_offset = Vector2(0.0f, 250.0f);

	GetTransform()->SetLocalPosition(m_target->GetLocalPosition() + m_offset);
}

void FollowCameraController::Update()
{
	GetTransform()->SetLocalPosition(m_target->GetLocalPosition() + m_offset);
}