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
	void SetEarthInfo(const std::wstring& earthInfo);
	void SetMoonInfo(const std::wstring& moonInfo);

private:
	std::wstring FormatBytes(UINT64 bytes);
};