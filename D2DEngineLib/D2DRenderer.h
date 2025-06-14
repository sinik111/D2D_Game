#pragma once

#include "Matrix3x2.h"

class D2DRenderer
{
private:
	HWND m_hWnd;
	
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3d11Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	UINT m_width;
	UINT m_height;

	Matrix3x2 m_unityMatrix;

public:
	D2DRenderer(HWND hWnd, UINT width, UINT height);
	~D2DRenderer();

public:
	void Initialize();

public:
	void BeginDraw(const D2D1::ColorF& color) const;
	void EndDraw() const;

	void DrawBitmap(const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& bitmap) const;
	void DrawBitmap(const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& bitmap, const D2D1_RECT_F& destinationRect) const;
	void DrawBitmap(const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& bitmap, const D2D1_RECT_F& destinationRect, const D2D1_RECT_F& sourceRect,
		float opacitiy = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR) const;
	void DrawBitmap(const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& bitmap, const D2D1_RECT_F& destinationRect, const D2D1_RECT_F& sourceRect,
		const Matrix3x2& transform, float opacitiy = 1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR) const;

	void DrawBitmap(ID2D1Bitmap1* bitmap, const Matrix3x2& transform, const D2D1_RECT_F* offset = nullptr
		, float opacitiy = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR) const;

public:
	const Microsoft::WRL::ComPtr<ID2D1DeviceContext7>& GetDeviceContext() const;
	Matrix3x2 GetUnityMatrix() const;
};