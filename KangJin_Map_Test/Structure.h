#pragma once
#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/json.hpp"
class Structure : public Script
{
protected :
	std::string jsonfilepath;
	bool isTransparent = false;
	std::vector<Vector2> m_colliderPoints;
	float scale = 1.0f;
public :
	std::vector<CircleCollider*> m_Colliders;
	int index = 1;

	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	
	void SetSize(int size) { index = size ; }

	void OnTriggerStay(const Collision& collision) override;

	virtual void ImportfromJson();
	virtual void ExporttoJson();
};
