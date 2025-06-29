#include "pch.h"
#include "D2DRenderer.h"

#include <algorithm>

#include "Matrix3x2.h"
#include "IRenderer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")

using Microsoft::WRL::ComPtr;

D2DRenderer::D2DRenderer(HWND hWnd, UINT width, UINT height)
	: m_hWnd{ hWnd }, m_width{ width }, m_height{ height }
{
	
}

D2DRenderer::~D2DRenderer()
{
	
}

HRESULT D2DRenderer::Initialize()
{
	HRESULT hr = S_OK;

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

	ComPtr<IDXGIDevice> dxgiDevice;
	m_d3d11Device.As(&dxgiDevice);

	ComPtr<ID2D1Device7> d2dDevice;
	d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
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

	// DWrite Factory
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dWriteFactory),
		reinterpret_cast<IUnknown**>(m_dWriteFactory.GetAddressOf())
	);

	// DWrite Text Format


	// Brush
	m_d2dDeviceContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		m_d2dSolidColorBrush.GetAddressOf()
	);

	// Unity 스타일 좌표계용 Matrix
	m_unityMatrix = Matrix3x2::Scale(1.0f, -1.0f) *
		Matrix3x2::Translation(m_width / 2.0f, m_height / 2.0f);
}

void D2DRenderer::Shutdown()
{

}

UINT D2DRenderer::GetWidth() const
{
	return m_width;
}

UINT D2DRenderer::GetHeight() const
{
	return m_height;
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
		L"",
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
	m_renderQueue.push_back(renderer);
}

void D2DRenderer::AddRenderCommand(std::unique_ptr<IRenderCommand> renderCommand)
{
	m_renderCommands.push_back(std::move(renderCommand));
}

void D2DRenderer::PrepareRenderCommands()
{
	std::sort(
		m_renderCommands.begin(),
		m_renderCommands.end(),
		[](const std::unique_ptr<IRenderCommand>& a, const std::unique_ptr<IRenderCommand>& b) {
			return a->GetSortOrder() < b->GetSortOrder();
		}
	);

}

void D2DRenderer::PrepareRenderQueue()
{
}

void D2DRenderer::ExecuteRenderCommands()
{
	PrepareRenderCommands();

	// screen, world 분리 필요
	for (const auto& command : m_renderCommands)
	{
		switch (command->GetType())
		{
		case RenderCommandType::Bitmap:
		{
			BitmapRenderCommand* bitmapCmd = static_cast<BitmapRenderCommand*>(command.get());

			m_d2dDeviceContext->SetTransform(static_cast<D2D1_MATRIX_3X2_F>(bitmapCmd->transform));
			m_d2dDeviceContext->DrawBitmap(bitmapCmd->bitmap.Get(), nullptr,
				bitmapCmd->opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &bitmapCmd->sourceRect);
		}
			break;
		case RenderCommandType::Text:
		{
			TextRenderCommand* textCmd = static_cast<TextRenderCommand*>(command.get());

			D2D1_RECT_F layoutRect
			{
				textCmd->point.x,
				textCmd->point.y,
				textCmd->point.x + textCmd->size.width,
				textCmd->point.y + textCmd->size.height
			};

			m_d2dSolidColorBrush->SetColor(textCmd->color);
			m_d2dDeviceContext->SetTransform(static_cast<D2D1_MATRIX_3X2_F>(textCmd->transform));
			m_d2dDeviceContext->DrawTextW(textCmd->text.c_str(), static_cast<UINT32>(textCmd->text.size()),
				textCmd->textFormat.Get(), layoutRect, m_d2dSolidColorBrush.Get());
		}
			break;
		default:
			break;
		}
	}

	ClearCommands();
}

void D2DRenderer::ExecuteRenderQueue()
{
}

void D2DRenderer::ClearCommands()
{
	m_renderCommands.clear();
}

void D2DRenderer::ClearQueue()
{
}
