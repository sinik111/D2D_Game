#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/json.hpp"

class Maps : public Script
{
	std::vector<GameObject*> m_GameObjects;
	std::vector<Vector2> m_Positions;

public:
	void Initialize() override;
	void Start()override;
	void FixedUpdate()override;
	void Update()override;
	void LateUpdate()override;
	void OnDestroy()override;

	//void OnCollisionEnter(const Collision& collision)override;
	//void OnCollisionStay(const Collision& collision)override;
	//void OnCollisionExit(const Collision& collision)override;
	//void OnTriggerEnter(const Collision& collision)override;
	//void OnTriggerStay(const Collision& collision)override;
	//void OnTriggerExit(const Collision& collision)override;

	std::string ConvertPositiontoJSON();
	void ExportJsontoPath(std::string filepath);
	void CreateLineCollider();
};

