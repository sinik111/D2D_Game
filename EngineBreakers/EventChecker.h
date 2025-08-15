#pragma once
#include "../D2DEngineLib/Script.h"
class GameObject;
class CircleCollider;
class Camera;
class EventChecker
	: public Script
{
	Vector2 EventCheckPos;
	Vector2 EventEndPos;
	Vector2 EventPlayerPos;
	Vector2 JumpStartPos;
	GameObject* m_GameObject;
	Player* player;
	CircleCollider* m_collider;
	Camera* camera;

	float eventTimer = 0;
	bool isMoving = false;
	bool isJumpUp = false;
	bool isJumping = false;
	//bool i

	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerStay(const Collision& collision) override;
	void OnTriggerExit(const Collision& collision) override;

	int GetDirectionIndex(Vector2 direction);
public:
	void SetEventCheckPosition(Vector2 pos);
	void SetEventEndPosition(Vector2 pos);
	void SetEventJumpStartPosition(Vector2 pos);

	void SetPlayer(GameObject* player) { m_GameObject = player; }
	void SetIsJumpUp(bool value) { isJumpUp = value; }
	void SetCamera(Camera* cam) { camera = cam; }
};

