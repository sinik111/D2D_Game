#pragma once

#include "..\D2DEngineLib\Script.h"

class TextRenderer;

class InfoViewer :
	public Script
{
private:
	GameObject* m_sun = nullptr;
	TextRenderer* m_textRenderer = nullptr;
	std::wstring m_earthRotation;
	std::wstring m_moonRotation;

private:
	void Start() override;
	void LateUpdate() override;

public:
	void SetSpaceObjects(GameObject* sun);
	void SetEarthRotation(const std::wstring& rotationString);
	void SetMoonRotation(const std::wstring& rotationString);
};