#include "../D2DEngineLib/framework.h"
#include "DemoGameApp.h"

#include <cassert>

#pragma comment(lib,"windowscodecs.lib")

using Microsoft::WRL::ComPtr;

void DemoGameApp::Initialize()
{
	m_ClassStyle = CS_HREDRAW | CS_VREDRAW;
	m_WindowStyle = WS_OVERLAPPEDWINDOW;//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_hCursor = LoadCursorW(NULL, IDC_ARROW);

	m_X = CW_USEDEFAULT;
	m_Y = CW_USEDEFAULT;
	m_Width = 800;
	m_Height = 600;

	m_ClassName = L"D2DGame";
	m_WindowName = L"DemoGameApp";

	__super::Initialize();

	HRESULT hr;
	// Create WIC factory
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_pWICImagingFactory), (void**)m_pWICImagingFactory.GetAddressOf());

	hr = CreateBitmapFromFile(L"Resource/mushroom.png", m_pD2DBitmapFromFile.GetAddressOf());
	assert(SUCCEEDED(hr));

	m_IsRunning = true;
}

void DemoGameApp::Shutdown()
{
	m_pD2DBitmapFromFile = nullptr;
	m_pWICImagingFactory = nullptr;

	__super::Shutdown();
}

void DemoGameApp::Render()
{
	m_pRenderer->BeginDraw(D2D1::ColorF::DarkOliveGreen);

	m_pRenderer->DrawBitmap(m_pD2DBitmapFromFile);

	D2D1_SIZE_F size = m_pD2DBitmapFromFile->GetSize();

	D2D1_VECTOR_2F destPos1{ 100.0f, 0.0f };
	D2D1_RECT_F destRect1{ destPos1.x, destPos1.y, destPos1.x + size.width - 1, destPos1.y + size.height - 1 };

	m_pRenderer->DrawBitmap(m_pD2DBitmapFromFile, destRect1);

	m_pRenderer->EndDraw();
}

void DemoGameApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	__super::MessageProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			m_IsRunning = false;
		}
		break;
	}
}

HRESULT DemoGameApp::CreateBitmapFromFile(const std::wstring& path, ID2D1Bitmap1** outBitmap)
{
	ComPtr<IWICBitmapDecoder> decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter> converter;

	// ① 디코더 생성
	HRESULT hr = m_pWICImagingFactory->CreateDecoderFromFilename(
		path.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr))
	{
		return hr;
	}

	// ② 첫 프레임 얻기
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr))
	{
		return hr;
	}

	// ③ 포맷 변환기 생성
	hr = m_pWICImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr))
	{
		return hr;
	}

	// ④ GUID_WICPixelFormat32bppPBGRA로 변환
	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(hr))
	{
		return hr;
	}

	// ⑤ Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// ⑥ DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	hr = m_pRenderer->GetDeviceContext()->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);

	return hr;
}