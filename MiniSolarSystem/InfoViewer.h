#pragma once

#include "..\D2DEngineLib\Script.h"

class InfoViewer :
	public Script
{
private:
	std::vector<GameObject*> m_spaceObjects;

public:
	~InfoViewer() override = default;

private:
	void Start() override;
	void LateUpdate() override;

public:
	void SetSpaceObjects(std::vector<GameObject*> spaceObjects);
};