#pragma once

#include "..\D2DEngineLib\Script.h"

class InfoViewer :
	public Script
{
private:
	std::vector<GameObject*> m_spaceObjects;
	float m_earthRotation;
	float m_moonRotation;

private:
	void Start() override;
	void LateUpdate() override;

public:
	void SetSpaceObjects(std::vector<GameObject*> spaceObjects);
	void SetEarthRotation(float rotation);
	void SetMoonRotation(float rotation);
};