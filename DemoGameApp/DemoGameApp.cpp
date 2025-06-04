#include "../D2DEngineLib/framework.h"
#include "DemoGameApp.h"

#include <assert.h>

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

	// 0,0 위치에 비트맵 그대로 그리기
	DrawOriginal();

	// 원하는 영역에 그리기
	DrawDestApplied();

	// 원하는 영역에 원하는 부분 그리기
	DrawDestSrcApplied();

	// 변환행렬 적용
	DrawTransformApplied();

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

void DemoGameApp::DrawOriginal()
{
	m_pRenderer->DrawBitmap(m_pD2DBitmapFromFile);
}

void DemoGameApp::DrawDestApplied()
{
	D2D1_SIZE_F size = m_pD2DBitmapFromFile->GetSize();

	D2D1_VECTOR_2F destPos{ 100.0f, 0.0f };
	D2D1_RECT_F destRect{ destPos.x, destPos.y, destPos.x + size.width - 1, destPos.y + size.height - 1 };

	m_pRenderer->DrawBitmap(m_pD2DBitmapFromFile, destRect);
}

void DemoGameApp::DrawDestSrcApplied()
{
	D2D1_SIZE_F size = m_pD2DBitmapFromFile->GetSize();

	D2D1_VECTOR_2F srcPos{ size.width / 2, size.width / 2 };
	D2D1_RECT_F srcRect{ srcPos.x, srcPos.y, srcPos.x + size.width / 2 - 1, srcPos.y + size.height / 2 - 1 };

	D2D1_VECTOR_2F destPos{ 200.0f, 200.0f };
	D2D1_RECT_F destRect{ destPos.x, destPos.y, destPos.x + size.width / 2 - 1, destPos.y + size.height / 2 - 1 };

	m_pRenderer->DrawBitmap(m_pD2DBitmapFromFile, destRect, srcRect);
}

void DemoGameApp::DrawTransformApplied()
{
	D2D1_MATRIX_3X2_F transform{};

	transform = D2D1::Matrix3x2F::Scale(3.0f, 3.0f) *
		D2D1::Matrix3x2F::Rotation(90.0f) *
		D2D1::Matrix3x2F::Translation(300.0f, 400.0f);

	m_pRenderer->DrawBitmap(m_pD2DBitmapFromFile, transform);

	// 이미지 중앙 기준으로 변환하기
	D2D1_RECT_F centeredRect{ MakeCenteredRect() };

	transform = D2D1::Matrix3x2F::Scale(2.0f, 2.0f) *
		D2D1::Matrix3x2F::Rotation(-45.0f) *
		D2D1::Matrix3x2F::Translation(700.0f, 500.0f);

	m_pRenderer->DrawBitmap(m_pD2DBitmapFromFile, transform, &centeredRect);

	// 중점 확인용
	transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f) *
		D2D1::Matrix3x2F::Rotation(0.0f) *
		D2D1::Matrix3x2F::Translation(700.0f, 500.0f);

	m_pRenderer->DrawBitmap(m_pD2DBitmapFromFile, transform, &centeredRect);
}

D2D1_RECT_F DemoGameApp::MakeCenteredRect()
{
	D2D1_SIZE_F size = m_pD2DBitmapFromFile->GetSize();

	D2D1_VECTOR_2F centeredPos{ -size.width / 2, -size.height / 2 };

	return { centeredPos.x, centeredPos.y, centeredPos.x + size.width - 1, centeredPos.y + size.height - 1 };
}