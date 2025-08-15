#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Rigidbody2D.h"

#include "Player.h"
#include "EnemyBase.h"
#include "PlayerGrabAttackState.h"

PlayerGrabAttackState::PlayerGrabAttackState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerGrabAttackState::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	m_grabMoveEnd = false;

	m_grabAnimationTimer = 0.0f;

	context.boolParams[L"Invincibility"] = true;

	m_originPos = m_player->GetTransform()->GetLocalPosition();

	Debug::Log("그랩 공격 발사");

	if (m_player->m_grabAttackTarget != nullptr)
	{
		
		m_player->m_grabAttackTarget->MoveSpeed() = 0.0f;
		m_player->m_grabAttackTarget->MovingDestPos() = m_player->m_grabAttackTarget->GetTransform()->GetLocalPosition();

		Vector2 playerPos = m_player->GetTransform()->GetLocalPosition();
		Vector2 targetPos = m_player->m_grabAttackTarget->GetTransform()->GetLocalPosition();

		
		float minDistanceSq = FLT_MAX;

		// 8방향 정점 계산 및 가장 가까운 지점 찾기
		for (int i = 0; i < 8; ++i)
		{
			// 45도씩 회전하며 8방향 벡터를 생성
			float angle = 45.0f * i;
			float radian = angle * (3.141592f / 180.0f);

			Vector2 offset(cos(radian) * 200.0f, sin(radian) * 100.0f);
			Vector2 tempPos = targetPos + offset;

			float distanceSq = (tempPos - playerPos).LengthSq();

			if (distanceSq < minDistanceSq)
			{
				minDistanceSq = distanceSq;
				m_player->m_grabMoveToPosition = tempPos;
			}
		}
	}
}

void PlayerGrabAttackState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;

	if (m_player->m_grabAttackTarget != nullptr && !m_grabMoveEnd)
	{
		Vector2 playerPos = m_player->GetTransform()->GetLocalPosition();
		Vector2 targetPos = m_player->m_grabMoveToPosition; // 타겟 위치 변경

		Vector2 direction = targetPos - playerPos;
		float distanceSq = direction.LengthSq();

		Debug::Log(playerPos, "  /  ", targetPos, "  /  ", distanceSq);

		if (distanceSq <= 10.0f)
		{
			m_player->m_currPlayerSpeed = 0.0f;
			context.rigidBody2d->SetVelocity(Vector2::Zero);
			m_grabMoveEnd = true;
			Debug::Log("End!!--  ", playerPos, "  /  ", targetPos, "  /  ", distanceSq);
		}
		else
		{
			const float moveSpeed = 700.0f;
			direction.Normalize();
			m_player->m_currPlayerSpeed = moveSpeed;			
		}
		return;
	}

	if (m_grabMoveEnd)
	{
		m_player->SetAnimState(Player::PlayerAnimGrabAttack);
		m_player->ForceHitStop(0.01f, 1.8f);
	}

	m_grabAnimationTimer += MyTime::UnscaledDeltaTime();
	
	if (m_grabAnimationTimer >= m_grabAnimationDuration)
	{		
		m_isStateChanged = true;
		m_player->ChangeStateWalkOrIdle();	
		return;
	}
}

void PlayerGrabAttackState::Exit(FSMContext& context)
{
	context.boolParams[L"Invincibility"] = false;
	m_player->m_isReadyGrabAttack = false;
	m_player->m_grabAttackTarget = nullptr;

	//m_player->GetTransform()->SetLocalPosition(m_originPos);
	
}