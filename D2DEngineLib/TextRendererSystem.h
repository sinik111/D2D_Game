#pragma once

class D2DRenderer;
class TextRenderer;

class TextRendererSystem
{
private:
	D2DRenderer* m_d2dRenderer = nullptr;
	std::vector<TextRenderer*> m_textRenderers;

public:
	void Register(TextRenderer* textRenderer);
	void Unregister(TextRenderer* textRenderer);

public:
	void SetD2DRenderer(D2DRenderer* d2dRenderer);
	void Update();
	Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat(float fontSize);
};