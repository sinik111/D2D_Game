#pragma once

#include "../D2DEngineLib/Script.h"

class Sun :
	public Script
{
private:
	std::list<GameObject*> m_earths;
	float m_speed;

public:
	~Sun() override = default;

private:
	void Start() override;
	void Update() override;

	void CreateEarth();
	void DeleteEarth();

public:
	size_t GetEarthCount() const;
};