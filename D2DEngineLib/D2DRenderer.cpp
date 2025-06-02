#include "pch.h"
#include "D2DRenderer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

D2DRenderer::D2DRenderer(HWND hWnd, UINT width, UINT height)
	: m_hWnd(hWnd), m_Width(width), m_Height(height)
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
		D3D11_SDK_VERSION, m_pD3D11Device.GetAddressOf(), &featureLevel, nullptr);

	// D2D 팩토리 및 디바이스
	ComPtr<ID2D1Factory8> pD2DFactory;
	D2D1_FACTORY_OPTIONS options =
	{
#ifdef _DEBUG
		D2D1_DEBUG_LEVEL_INFORMATION
#endif // _DEBUG
	};

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, pD2DFactory.GetAddressOf());

	ComPtr<IDXGIDevice> pDXGIDevice;
	m_pD3D11Device.As(&pDXGIDevice);

	ComPtr<ID2D1Device7> pD2DDevice;
	pD2DFactory->CreateDevice(pDXGIDevice.Get(), pD2DDevice.GetAddressOf());
	pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_pD2DDeviceContext.GetAddressOf());

	ComPtr<IDXGIFactory7> pDXGIFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&pDXGIFactory));

	// SwapChain 생성
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = m_Width;
	swapChainDesc.Height = m_Height;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	pDXGIFactory->CreateSwapChainForHwnd(m_pD3D11Device.Get(), m_hWnd,
		&swapChainDesc, nullptr, nullptr, m_pDXGISwapChain.GetAddressOf());

	// SwapChain의 BackBuffer를 사용하는 D2D1Bitmap1 인터페이스 생성
	ComPtr<IDXGISurface> pBackBuffer;
	m_pDXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(swapChainDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	m_pD2DDeviceContext->CreateBitmapFromDxgiSurface(pBackBuffer.Get(),
		&bmpProps, m_pD2DBitmapTarget.GetAddressOf());
	m_pD2DDeviceContext->SetTarget(m_pD2DBitmapTarget.Get());
}

void D2DRenderer::BeginDraw(const D2D1::ColorF& color) const
{
	m_pD2DDeviceContext->BeginDraw();
	m_pD2DDeviceContext->Clear(D2D1::ColorF(color));
}

void D2DRenderer::EndDraw() const
{
	m_pD2DDeviceContext->EndDraw();
	m_pDXGISwapChain->Present(1, 0);
}

void D2DRenderer::DrawBitmap(const ComPtr<ID2D1Bitmap1>& pBitmap) const
{
	m_pD2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pD2DDeviceContext->DrawBitmap(pBitmap.Get());
}

void D2DRenderer::DrawBitmap(const ComPtr<ID2D1Bitmap1>& pBitmap, const D2D1_RECT_F& destRect) const
{
	m_pD2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pD2DDeviceContext->DrawBitmap(pBitmap.Get(), destRect);
}

void D2DRenderer::DrawBitmap(const ComPtr<ID2D1Bitmap1>& pBitmap, const D2D1_RECT_F& destRect, const D2D1_RECT_F& srcRect,
	float opacitiy, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode) const
{
	m_pD2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pD2DDeviceContext->DrawBitmap(pBitmap.Get(), destRect, opacitiy, interpolationMode, srcRect);
}

const ComPtr<ID2D1DeviceContext7>& D2DRenderer::GetDeviceContext() const
{
	return m_pD2DDeviceContext;
}