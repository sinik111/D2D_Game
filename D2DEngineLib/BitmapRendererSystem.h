#pragma once

class D2DRenderer;
class BitmapRenderer;

class BitmapRendererSystem
{
private:
	D2DRenderer* m_d2dRenderer;
	std::vector<BitmapRenderer*> m_bitmapRenderers;

public:
	void Register(BitmapRenderer* bitmapRenderer);
	void Unregister(BitmapRenderer* bitmapRenderer);

public:
	void SetD2DRenderer(D2DRenderer* d2dRenderer);
	void UpdateSystem();
};