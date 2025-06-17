#pragma once

#include "../D2DEngineLib/Script.h"

class GameObject;

class Sun :
	public Script
{
private:
	std::list<GameObject*> m_earths;

public:
	Sun();
	~Sun() override = default;

private:
	void Start() override;
	void Update() override;
};