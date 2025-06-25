#pragma once

#include "../D2DEngineLib/Script.h"

class TextRenderer;

class HpViewer :
	public Script
{
private:
	std::wstring m_targetName;
	TextRenderer* m_textRenderer{};

private:
	void Initialize() override;
	void Start() override;

public:
	void ChangeHpText(int prevHp, int currHp, int maxHp);
	void SetTargetName(const std::wstring& targetName);
};