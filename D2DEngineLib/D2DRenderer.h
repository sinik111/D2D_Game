#pragma once

#include "Matrix3x2.h"
#include "Enum.h"

class IRenderer;

class D2DRenderer
{
private:
	HWND m_hWnd;
	
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3d11Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;
	Microsoft::WRL::ComPtr<IDXGIDevice3> m_dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter3> m_dxgiAdapter;

	// DWrite
	Microsoft::WRL::ComPtr<IDWriteFactory> m_dWriteFactory;

	// Brush
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_d2dSolidColorBrush;
	std::vector<std::vector<std::vector<IRenderer*>>> m_renderQueues;

	UINT m_width;
	UINT m_height;

	Matrix3x2 m_unityMatrix;

public:
	D2DRenderer(HWND hWnd, UINT width, UINT height);

public:
	HRESULT Initialize();

	UINT GetWidth() const;
	UINT GetHeight() const;

public:
	void BeginDraw(const D2D1::ColorF& color) const;
	void EndDraw() const;

public:
	const Microsoft::WRL::ComPtr<ID2D1DeviceContext7>& GetDeviceContext() const;
	Matrix3x2 GetUnityMatrix() const;

public:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat(float fontSize);

public:
	void RegisterRendererToQueue(IRenderer* renderer);
	void ExecuteRenderQueue();

	void Trim();
	void DrawRect(const D2D1_RECT_F& rect);

private:
	void PrepareRenderQueue();
	void ClearQueue();
};