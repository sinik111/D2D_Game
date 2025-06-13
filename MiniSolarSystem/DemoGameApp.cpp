#include "../D2DEngineLib/framework.h"
#include "DemoGameApp.h"

#include <cassert>

#include "Camera.h"
#include "SpaceObject.h"
#include "../D2DEngineLib/Transform.h"

#pragma comment(lib,"windowscodecs.lib")

using Microsoft::WRL::ComPtr;

void DemoGameApp::Initialize()
{
	m_classStyle = CS_HREDRAW | CS_VREDRAW;
	m_windowStyle = WS_OVERLAPPEDWINDOW;//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_hCursor = LoadCursorW(NULL, IDC_ARROW);

	m_x = CW_USEDEFAULT;
	m_y = CW_USEDEFAULT;
	m_width = 800;
	m_height = 600;

	m_className = L"D2DGame";
	m_windowName = L"MiniSolarSystem";

	__super::Initialize();

	m_camera = new Camera();


	HRESULT hr;
	// Create WIC factory
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory), (void**)m_wicImagingFactory.GetAddressOf());

	m_sun = new SpaceObject(m_camera, m_renderer, 0.3f);
	m_earth = new SpaceObject(m_camera, m_renderer, 0.6f);
	m_moon = new SpaceObject(m_camera, m_renderer, 0.9f);

	// ID2D1Bitmap1** 에 직접 이미지 로드하는 방법

	//hr = CreateBitmapFromFile(L"Resource/Sun.png", m_sun->GetImage()->GetBitmap());
	//assert(SUCCEEDED(hr));

	//hr = CreateBitmapFromFile(L"Resource/Moon.png", m_moon->GetImage()->GetBitmap());
	//assert(SUCCEEDED(hr));

	//hr = CreateBitmapFromFile(L"Resource/Earth.png", m_earth->GetImage()->GetBitmap());
	//assert(SUCCEEDED(hr));

	// ComPtr로 로드 후 대입해서 넘겨주는 방법
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;

	hr = CreateBitmapFromFile(L"Resource/Sun.png", bitmap.GetAddressOf());
	assert(SUCCEEDED(hr));

	m_sun->GetImage()->SetBitmap(bitmap);
	bitmap = nullptr;

	hr = CreateBitmapFromFile(L"Resource/Moon.png", bitmap.GetAddressOf());
	assert(SUCCEEDED(hr));

	m_moon->GetImage()->SetBitmap(bitmap);
	bitmap = nullptr;

	hr = CreateBitmapFromFile(L"Resource/Earth.png", bitmap.GetAddressOf());
	assert(SUCCEEDED(hr));

	m_earth->GetImage()->SetBitmap(bitmap);
	bitmap = nullptr;

	m_sun->GetTransform()->SetScale(0.2f, 0.2f);
	m_moon->GetTransform()->SetScale(0.5f, 0.5f);

	m_earth->GetTransform()->SetPosition({ 800.0f, 0.0f });
	m_moon->GetTransform()->SetPosition({ 400.0f, 0.0f });

	m_earth->GetTransform()->SetParent(m_sun->GetTransform());
	m_moon->GetTransform()->SetParent(m_earth->GetTransform());

	m_isRunning = true;
}

void DemoGameApp::Shutdown()
{
	delete m_earth;
	delete m_moon;
	delete m_sun;
	delete m_camera;

	m_wicImagingFactory = nullptr;

	__super::Shutdown();
}

void DemoGameApp::Update()
{
	m_camera->Update();
	m_sun->Update();
	m_earth->Update();
	m_moon->Update();
}

void DemoGameApp::Render()
{
	m_renderer->BeginDraw(D2D1::ColorF::Black);

	m_sun->Render();
	m_earth->Render();
	m_moon->Render();

	m_renderer->EndDraw();
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
			m_isRunning = false;
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
	HRESULT hr = m_wicImagingFactory->CreateDecoderFromFilename(
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
	hr = m_wicImagingFactory->CreateFormatConverter(&converter);
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
	hr = m_renderer->GetDeviceContext()->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);

	return hr;
}