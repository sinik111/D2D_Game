#pragma once

#include "..\D2DEngineLib\Script.h"

class TextRenderer;

class InfoViewer :
	public Script
{
private:
	GameObject* m_sun;
	TextRenderer* m_textRenderer;
	float m_earthRotation;
	float m_moonRotation;

private:
	void Start() override;
	void LateUpdate() override;

public:
	void SetSpaceObjects(GameObject* sun);
	void SetEarthRotation(float rotation);
	void SetMoonRotation(float rotation);
};