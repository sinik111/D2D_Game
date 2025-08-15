#pragma once

#include "PlayerStateBase.h"

class PlayerGrabAttackState :
	public PlayerStateBase
{
private:
	float m_grabAnimationDuration = 1.7501f;
	float m_grabAnimationTimer = 0.0f;

	Vector2 m_originPos;

	bool m_grabMoveEnd = false;
public:
	PlayerGrabAttackState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};