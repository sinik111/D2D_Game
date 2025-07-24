#include "pch.h"
#include "D2DRenderer.h"

#include <algorithm>

#include "Matrix3x2.h"
#include "IRenderer.h"
#include "Camera.h"
#include "DebugSystem.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")

constexpr int MAX_SORT_ORDER = 30;

using Microsoft::WRL::ComPtr;

HRESULT D2DRenderer::Initialize(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr = S_OK;

	m_hWnd = hWnd;
	m_width = width;
	m_height = height;

	m_renderQueues.resize(static_cast<int>(SpaceType::MAX));
	for (auto& queue : m_renderQueues)
	{
		queue.resize(MAX_SORT_ORDER);
	}

	// D3D11 디바이스 생성
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[]{ D3D_FEATURE_LEVEL_11_0 };
	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		levels,
		1,
		D3D11_SDK_VERSION,
		m_d3d11Device.GetAddressOf(),
		&featureLevel,
		nullptr
	);

	// D2D 팩토리 및 디바이스
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options{
#ifdef _DEBUG
		D2D1_DEBUG_LEVEL_INFORMATION
#endif // _DEBUG
	};

	D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		options,
		d2dFactory.GetAddressOf()
	);

	m_d3d11Device.As(&m_dxgiDevice);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	m_dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
	dxgiAdapter.As(&m_dxgiAdapter);

	ComPtr<ID2D1Device7> d2dDevice;
	d2dFactory->CreateDevice(m_dxgiDevice.Get(), d2dDevice.GetAddressOf());
	d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		m_d2dDeviceContext.GetAddressOf()
	);

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
	dxgiFactory->CreateSwapChainForHwnd(
		m_d3d11Device.Get(),
		m_hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		m_dxgiSwapChain.GetAddressOf()
	);

	// SwapChain의 BackBuffer를 사용하는 D2D1Bitmap1 인터페이스 생성
	ComPtr<IDXGISurface> backBuffer;
	m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(
			swapChainDesc.Format,
			D2D1_ALPHA_MODE_PREMULTIPLIED
		)
	);

	m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
		backBuffer.Get(),
		&bmpProps, m_d2dBitmapTarget.GetAddressOf()
	);

	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());

	// Batch
	m_d2dDeviceContext->CreateSpriteBatch(m_d2dSpriteBatch.GetAddressOf());

	// DWrite Factory
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dWriteFactory),
		reinterpret_cast<IUnknown**>(m_dWriteFactory.GetAddressOf())
	);

	// Brush
	m_d2dDeviceContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		m_d2dSolidColorBrush.GetAddressOf()
	);

	// Unity 스타일 좌표계용 Matrix
	m_unityMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
		Matrix3x2::Translation(m_width / 2.0f, m_height / 2.0f);

	D2D1_MATRIX_3X2_F a;
	D2D1_MATRIX_3X2_F b;
	D2D1_MATRIX_3X2_F c = a * b;

	return S_OK;
}

UINT D2DRenderer::GetWidth() const
{
	return m_width;
}

UINT D2DRenderer::GetHeight() const
{
	return m_height;
}

void D2DRenderer::BeginDraw(const D2D1_COLOR_F& color) const
{
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->Clear(color);
}

void D2DRenderer::EndDraw() const
{
	m_d2dDeviceContext->EndDraw();
	m_dxgiSwapChain->Present(1, 0);

	DXGI_QUERY_VIDEO_MEMORY_INFO memInfo{};
	m_dxgiAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);

	DebugSystem::Get().SetVRAMCurrentUsage(memInfo.CurrentUsage);
}

const ComPtr<ID2D1DeviceContext7>& D2DRenderer::GetDeviceContext() const
{
	return m_d2dDeviceContext;
}

Matrix3x2 D2DRenderer::GetUnityMatrix() const
{
	return m_unityMatrix;
}

ComPtr<IDWriteTextFormat> D2DRenderer::CreateTextFormat(float fontSize)
{
	ComPtr<IDWriteTextFormat> newDWriteTextFormat;

	m_dWriteFactory->CreateTextFormat(
		L"consolas",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"",
		newDWriteTextFormat.GetAddressOf()
	);

	return newDWriteTextFormat;
}

void D2DRenderer::RegisterRendererToQueue(IRenderer* renderer)
{
	m_renderQueues[static_cast<int>(renderer->GetSpaceType())][renderer->GetSortOrder()].push_back(renderer);
}

void D2DRenderer::PrepareRenderQueue()
{
	for (auto& spaceTypeGroup : m_renderQueues)
	{
		for (auto& sortOrderGroup : spaceTypeGroup)
		{
			std::sort(
				sortOrderGroup.begin(),
				sortOrderGroup.end(),
				[](const IRenderer* a, const IRenderer* b) {
					return a->GetY() > b->GetY();
				}
			);
		}
	}
}

void D2DRenderer::ExecuteRenderQueue()
{
	PrepareRenderQueue();

	RenderContext context{
		m_unityMatrix,
		Camera::s_mainCamera->GetViewMatrix() * m_unityMatrix,
		m_d2dDeviceContext,
		m_d2dSolidColorBrush,
		m_d2dSpriteBatch
	};

	for (const auto& spaceTypeGroup : m_renderQueues)
	{
		for (const auto& sortOrderGroup : spaceTypeGroup)
		{
			for (const auto& renderer : sortOrderGroup)
			{
				renderer->Render(context);
			}
		}
	}

	ClearQueue();
}

void D2DRenderer::Trim()
{
	m_dxgiDevice->Trim();
}

void D2DRenderer::DrawRect(const D2D1_RECT_F& rect)
{
	m_d2dDeviceContext->SetTransform(Camera::s_mainCamera->GetViewMatrix() * m_unityMatrix);
	m_d2dSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	m_d2dDeviceContext->DrawRectangle(rect, m_d2dSolidColorBrush.Get());
}

void D2DRenderer::ClearQueue()
{
	for (auto& spaceTypeGroup : m_renderQueues)
	{
		for (auto& sortOrderGroup : spaceTypeGroup)
		{
			sortOrderGroup.clear();
		}
	}
}
