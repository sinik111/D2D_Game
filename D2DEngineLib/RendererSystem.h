#pragma once

class D2DRenderer;
class IRenderer;

class RendererSystem
{
private:
	D2DRenderer* m_d2dRenderer = nullptr;
	std::vector<IRenderer*> m_renderers;

public:
	void Register(IRenderer* textRenderer);
	void Unregister(IRenderer* textRenderer);

public:
	void SetD2DRenderer(D2DRenderer* d2dRenderer);
	void RegisterRendererToRenderQueue();
	Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat(float fontSize);
};