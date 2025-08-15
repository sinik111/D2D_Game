#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Camera.h"

#include "Player.h"
#include "EventChecker.h"
#include "PlayerCameraController.h"

void EventChecker::Start()
{
	m_collider = GetGameObject()->AddComponent<CircleCollider>();
	m_collider->SetLayer(CollisionLayer::Sensor);
	m_collider->SetTrigger(true);
	m_collider->SetRadius(400);
	m_collider->SetOffset(EventCheckPos);
	player = m_GameObject->GetComponent<Player>();
}

void EventChecker::Update()
{
	if (isMoving)
	{
		Vector2 eventDistance = JumpStartPos - EventCheckPos;
		player->m_JumpStartDirectionIndex = GetDirectionIndex(eventDistance);
		player->m_JumpStartDirection = eventDistance;
	}
	if (player->m_isJump && isJumping)
	{
		isJumping = false;
		if(!player->m_isJumptoUP)
		{
			JumpStartPos = EventCheckPos;
			Vector2 oneVector = { 0.7071f, -0.3536f };
			Vector2 destPos = player->GetTransform()->GetLocalPosition() + (oneVector * 200.0f);
			Camera::s_mainCamera->GetGameObject()->GetComponent<PlayerCameraController>()->StartLerp(JumpStartPos, destPos, 2.1f);
		}
		else
		{
			Vector2 threeVector = { -0.7071f, -0.3536f };
			Vector2 destPos = player->GetTransform()->GetLocalPosition() + (threeVector * 50.0f) + Vector2{ -80,0 };
			Camera::s_mainCamera->GetGameObject()->GetComponent<PlayerCameraController>()->StartLerp(JumpStartPos, destPos, 2.1f);
		}
	}
}

void EventChecker::FixedUpdate()
{
	
}

void EventChecker::OnTriggerEnter(const Collision& collision)
{
	player->m_isColliding = true;
	player->m_isJumptoUP = isJumpUp;
	player->m_landPos = EventEndPos;
	isJumping = true;
	//EventPlayerPos = player->GetTransform()->GetLocalPosition();
	//MovePos = EventCheckPos - EventPlayerPos;
}

void EventChecker::OnTriggerStay(const Collision& collision)
{
	//이 부분에다 상호작용 키 이미지 띄우기
	// 
	//if (player->m_isColliding == false)
	//{
	//	player->m_isColliding = true;
	//}

	//if (!isJumped && Input::IsKeyHeld('Z'))
	//{
	//	isJumped = true;
	//	m_GameObject->GetComponent<RigidBody2D>()->SetPosition(EventEndPos);

	//	//player->JumpKeyPressed();

	//	if (isJumpUp) { player->JumpUp(); }
	//	else { player->JumpDown(); }

	//	m_GameObject->GetComponent<CircleCollider>()->SetTrigger(true);
	//}
}

void EventChecker::OnTriggerExit(const Collision& collision)
{
	m_GameObject->GetComponent<CircleCollider>()->SetTrigger(false);
	player->m_isJump = false;
	player->m_isColliding = false;
}

int EventChecker::GetDirectionIndex(Vector2 direction)
{
	float minDegree = 360;

	float oneDirection = Vector2::AngleBetween({ -0.7071f, -0.3536f }, direction);
	float twoDirection = Vector2::AngleBetween(Vector2(0.0f, -1.0f) * 0.5f, direction);
	float threeDirection = Vector2::AngleBetween({ 0.7071f, -0.3536f }, direction);
	float fourDirection = Vector2::AngleBetween(Vector2(-1.0f, 0.0f), direction);
	float sixDirection = Vector2::AngleBetween(Vector2(1.0f, 0.0f), direction);
	float sevenDirection = Vector2::AngleBetween({ -0.7071f, 0.3536f }, direction);
	float eightDirection = Vector2::AngleBetween(Vector2(0.0f, 1.0f) * 0.5f, direction);
	float nineDirection = Vector2::AngleBetween({ 0.7071f, 0.3536f }, direction);

	if (minDegree > oneDirection){
		minDegree = oneDirection;}

	if (minDegree > twoDirection){
		minDegree = twoDirection;}

	if (minDegree > threeDirection){
		minDegree = threeDirection;}
	
	if (minDegree > fourDirection){
		minDegree = fourDirection;}
	
	if (minDegree > sixDirection){
		minDegree = sixDirection;}
	
	if (minDegree > sevenDirection){
		minDegree = sevenDirection;}
	
	if (minDegree > eightDirection){
		minDegree = eightDirection;}
	
	if (minDegree > nineDirection){
		minDegree = nineDirection;}

	if(minDegree == oneDirection){return 1;}
	else if(minDegree == twoDirection){return 2;}
	else if(minDegree == threeDirection){return 3;}
	else if(minDegree == fourDirection){return 4;}
	else if(minDegree == sixDirection){return 6;}
	else if(minDegree == sevenDirection){return 7;}
	else if(minDegree == eightDirection){return 8;}
	else if(minDegree == nineDirection){return 9;}
	else {return 0;}
}

void EventChecker::SetEventCheckPosition(Vector2 pos)
{
	EventCheckPos = pos;
}

void EventChecker::SetEventEndPosition(Vector2 pos)
{
	EventEndPos = pos;
}

void EventChecker::SetEventJumpStartPosition(Vector2 pos)
{
	JumpStartPos = pos;
}
