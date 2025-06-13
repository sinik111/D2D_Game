#include "pch.h"
#include "D2DRenderer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

D2DRenderer::D2DRenderer(HWND hWnd, UINT width, UINT height)
	: m_hWnd(hWnd), m_width(width), m_height(height)
{
}

D2DRenderer::~D2DRenderer()
{
	int a = 0;
}

void D2DRenderer::Initialize()
{
	// D3D11 디바이스 생성
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[]{ D3D_FEATURE_LEVEL_11_0 };
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, 1,
		D3D11_SDK_VERSION, m_d3d11Device.GetAddressOf(), &featureLevel, nullptr);

	// D2D 팩토리 및 디바이스
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options =
	{
#ifdef _DEBUG
		D2D1_DEBUG_LEVEL_INFORMATION
#endif // _DEBUG
	};

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	m_d3d11Device.As(&dxgiDevice);

	ComPtr<ID2D1Device7> d2dDevice;
	d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dDeviceContext.GetAddressOf());

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// SwapChain 생성
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = m_width;
	swapChainDesc.Height = m_height;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiFactory->CreateSwapChainForHwnd(m_d3d11Device.Get(), m_hWnd,
		&swapChainDesc, nullptr, nullptr, m_dxgiSwapChain.GetAddressOf());

	// SwapChain의 BackBuffer를 사용하는 D2D1Bitmap1 인터페이스 생성
	ComPtr<IDXGISurface> backBuffer;
	m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(swapChainDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(),
		&bmpProps, m_d2dBitmapTarget.GetAddressOf());
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());

	m_unityMatrix = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) *
		D2D1::Matrix3x2F::Translation((float)m_width / 2, (float)m_height / 2);
}

void D2DRenderer::BeginDraw(const D2D1::ColorF& color) const
{
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->Clear(D2D1::ColorF(color));
}

void D2DRenderer::EndDraw() const
{
	m_d2dDeviceContext->EndDraw();
	m_dxgiSwapChain->Present(1, 0);
}

void D2DRenderer::DrawBitmap(const ComPtr<ID2D1Bitmap1>& bitmap) const
{
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_d2dDeviceContext->DrawBitmap(bitmap.Get());
}

void D2DRenderer::DrawBitmap(const ComPtr<ID2D1Bitmap1>& bitmap, const D2D1_RECT_F& destinationRect) const
{
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_d2dDeviceContext->DrawBitmap(bitmap.Get(), destinationRect);
}

void D2DRenderer::DrawBitmap(const ComPtr<ID2D1Bitmap1>& bitmap, const D2D1_RECT_F& destinationRect, const D2D1_RECT_F& sourceRect,
	float opacitiy, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode) const
{
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_d2dDeviceContext->DrawBitmap(bitmap.Get(), destinationRect, opacitiy, interpolationMode, sourceRect);
}

void D2DRenderer::DrawBitmap(const ComPtr<ID2D1Bitmap1>& bitmap, const D2D1_RECT_F& destinationRect, const D2D1_RECT_F& sourceRect, const D2D1_MATRIX_3X2_F& transform, float opacitiy, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode) const
{
	m_d2dDeviceContext->SetTransform(transform);
	m_d2dDeviceContext->DrawBitmap(bitmap.Get(), destinationRect, opacitiy, interpolationMode, sourceRect);
}

void D2DRenderer::DrawBitmap(ID2D1Bitmap1* bitmap, const D2D1_MATRIX_3X2_F& transform, const D2D1_RECT_F* offset, float opacitiy, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode) const
{
	m_d2dDeviceContext->SetTransform(transform);
	m_d2dDeviceContext->DrawBitmap(bitmap, offset, opacitiy, interpolationMode);
}

const ComPtr<ID2D1DeviceContext7>& D2DRenderer::GetDeviceContext() const
{
	return m_d2dDeviceContext;
}

D2D1::Matrix3x2F D2DRenderer::GetUnityMatrix() const
{
	return m_unityMatrix;
}
