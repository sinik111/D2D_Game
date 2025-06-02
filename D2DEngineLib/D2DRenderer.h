#pragma once

class D2DRenderer
{
private:
	HWND m_hWnd;
	
	Microsoft::WRL::ComPtr<ID3D11Device> m_pD3D11Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pDXGISwapChain;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_pD2DDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_pD2DBitmapTarget;

	UINT m_Width;
	UINT m_Height;

public:
	D2DRenderer(HWND hWnd, UINT width, UINT height);
	~D2DRenderer();

public:
	void Initialize();

public:
	void BeginDraw(const D2D1::ColorF& color) const;
	void EndDraw() const;

	void DrawBitmap(const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& pBitmap) const;
	void DrawBitmap(const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& pBitmap, const D2D1_RECT_F& destRect) const;
	void DrawBitmap(const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& pBitmap, const D2D1_RECT_F& destRect, const D2D1_RECT_F& srcRect,
		float opacitiy = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR) const;

public:
	const Microsoft::WRL::ComPtr<ID2D1DeviceContext7>& GetDeviceContext() const;
};