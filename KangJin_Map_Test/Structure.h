#pragma once
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/Script.h"

class Structure : public Script
{
public:
	std::vector<CircleCollider*> m_Colliders;
	void Initialize() override;
	int index = 1;
	void SetSize(int size) { index = size+1; }

	void OnTriggerStay(const Collision& collision) override;
};

