#pragma once

#include "../D2DEngineLib/Script.h"

class Sun :
	public Script
{
private:
	std::list<GameObject*> m_earths;
	GameObject* m_batchTest = nullptr;
	float m_speed = 0.0f;

private:
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	void OnTriggerEnter(const Collision& collision) override;

	void CreateEarth();
	void DeleteEarth();
	void DestroySelf();

public:
	size_t GetEarthCount() const;
};