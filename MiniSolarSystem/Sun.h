#pragma once

#include "../D2DEngineLib/Script.h"

class Sun :
	public Script
{
private:
	std::list<GameObject*> m_earths;
	float m_speed = 0.0f;

private:
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	void CreateEarth();
	void DeleteEarth();
	void DestroySelf();

public:
	size_t GetEarthCount() const;
};