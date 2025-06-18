#pragma once

#include "Matrix3x2.h"
#include "RenderCommand.h"

class D2DRenderer
{
private:
	HWND m_hWnd;
	
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3d11Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	// DWrite
	Microsoft::WRL::ComPtr<IDWriteFactory> m_dWriteFactory;

	// Brush
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_d2dSolidColorBrush;

	std::vector<std::unique_ptr<IRenderCommand>> m_renderCommands;

	UINT m_width;
	UINT m_height;

	Matrix3x2 m_unityMatrix;

public:
	D2DRenderer(HWND hWnd, UINT width, UINT height);
	~D2DRenderer();

public:
	void Initialize();
	void Shutdown();

public:
	void BeginDraw(const D2D1::ColorF& color) const;
	void EndDraw() const;

public:
	const Microsoft::WRL::ComPtr<ID2D1DeviceContext7>& GetDeviceContext() const;
	Matrix3x2 GetUnityMatrix() const;

public:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> CreateTextFormat(float fontSize);

public:
	void AddRenderCommand(std::unique_ptr<IRenderCommand> renderCommand);
	void ExecuteRenderCommands();

private:
	void PrepareRenderCommands();
	void ClearCommands();
};