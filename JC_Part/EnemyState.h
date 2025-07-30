#pragma once
#include "../D2DEngineLib/IState.h"

class EnemyState 
	: public IState
{
public:

	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

	virtual void SetDirection(FSMContext& context);
	//virtual void UpdateMovement(FSMContext& context);
	
	int prevDir = 0;
	int curDir = 0;
	
	/*Vector2& m_pos;
	
	float& m_rotationAngle;            
	
	Vector2& m_cameraPos;
	
	Vector2& m_targetPos;
	
	Vector2& m_originPos;
	
	float& m_originAngle;
	
	Vector2& m_movingDestPos;
	   
	float& m_moveSpeed;
	float& m_maxSightDistance;
	float& m_sightAngle;
	float& m_maxRoamDistance;
	float& m_maxChaseDistance;
		
	int& m_direction;
	
	bool& m_isMove;
	bool& m_toDoMove;*/

	std::wstring animPath[10] = {
		L"",
		L"",
		L"",
		L""
		L"",
		L"",
		L"",
		L"",
		L"",
		L"",
	};

	//void Init(FSMContext& context)
	//{
	//	/*m_posX = context.floatParams[L"PosX"];
	//	m_posY = context.floatParams[L"PosY"];
	//	m_rotationAngle = context.floatParams[L"Angle"]; 
	//	m_direction = context.intParams[L"Direction"];
	//	
	//	m_isMove = context.boolParams[L"isMove"];
	//
	//	m_cameraPosX = context.floatParams[L"CameraPosX"];
	//	m_cameraPosY = context.floatParams[L"CameraPosY"];

	//	m_targetPosX = context.floatParams[L"TargetPosX"];
	//	m_targetPosY = context.floatParams[L"TargetPosY"];

	//	m_originPosX = context.floatParams[L"OriginPosX"];
	//	m_originPosY = context.floatParams[L"OriginPosY"];
	//	m_originAngle = context.floatParams[L"OriginAngle"];

	//	m_toDoMove = context.boolParams[L"ToDoMove"];
	//	
	//	m_movingDestPosX = context.floatParams[L"MovingDestPosX"];
	//	m_movingDestPosY = context.floatParams[L"MovingDestPosY"];

	//	m_moveSpeed = context.floatParams[L"MoveSpeed"];
	//	m_maxSightDistance = context.floatParams[L"MaxSightDistance"];
	//	m_sightAngle = context.floatParams[L"SightAngle"];
	//	m_maxRoamDistance = context.floatParams[L"MaxRoamDistance"];
	//	m_maxChaseDistance = context.floatParams[L"MaxChaseDistance"];*/
	//}

	

};

