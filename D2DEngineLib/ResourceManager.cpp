#include "pch.h"
#include "ResourceManager.h"

#include <cassert>

using Microsoft::WRL::ComPtr;

ResourceManager& ResourceManager::Get()
{
	static ResourceManager s_instance;

	return s_instance;
}

void ResourceManager::Initialize(ComPtr<ID2D1DeviceContext7> deviceContext,
	const std::wstring& modulePath,
	const std::wstring& binaryFolderName,
	const std::wstring& resourceFolderName)
{
	m_d2d1DeviceContext = deviceContext;

	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory), (void**)m_wicImagingFactory.GetAddressOf());

	bool isWrongDirectory = true;

	m_resourcePath = modulePath;

	size_t pos = m_resourcePath.length();

	while (pos != std::wstring::npos)
	{
		pos = m_resourcePath.rfind(L'\\', pos - 1);
		if (pos != std::wstring::npos)
		{
			std::wstring folderName = m_resourcePath.substr(pos + 1);
			if (folderName == binaryFolderName)
			{
				m_resourcePath = m_resourcePath.substr(0, pos); // 폴더 이름 제거
				isWrongDirectory = false;
				break;
			}

			m_resourcePath = m_resourcePath.substr(0, pos);
		}
	}

	if (isWrongDirectory)
	{
		return;
	}

	m_resourcePath += std::wstring(L"\\" + resourceFolderName + L"\\");

	return;
}

void ResourceManager::Release()
{
	for (auto& bitmaps : m_bitmapGroups)
	{
		bitmaps.second.clear();
	}

	m_bitmapGroups.clear();

	m_wicImagingFactory = nullptr;
	m_d2d1DeviceContext = nullptr;
}

void ResourceManager::ReleaseResources(const std::wstring group)
{
	m_bitmapGroups[group].clear();
}

HRESULT ResourceManager::LoadBitmapFromFile(const std::wstring group, const std::wstring name, const std::wstring& fileName)
{
	Bitmaps& bitmaps = m_bitmapGroups[group];

	if (bitmaps.find(name) != bitmaps.end())
	{
		assert(false && "같은 키 있음 - ResourceManager::LoadBitmapFromFile");

		return E_FAIL;
	}

	ComPtr<IWICBitmapDecoder> decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter> converter;

	HRESULT hr;

	// 디코더 생성
	hr = m_wicImagingFactory->CreateDecoderFromFilename(std::wstring(m_resourcePath + fileName).c_str(),
		nullptr,
		GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr))
	{
		return hr;
	}

	// 첫 프레임 얻기
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr))
	{
		return hr;
	}

	// 포맷 변환기 생성
	hr = m_wicImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr))
	{
		return hr;
	}

	// GUID_WICPixelFormat32bppPBGRA로 변환
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

	// Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps{
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_NONE,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED))
	};

	ComPtr<ID2D1Bitmap1> bitmap;

	// DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	hr = m_d2d1DeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, bitmap.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	bitmaps.emplace(name, bitmap);

	return hr;
}

ComPtr<ID2D1Bitmap1> ResourceManager::GetBitmap(const std::wstring group, const std::wstring name)
{
	Bitmaps& bitmaps = m_bitmapGroups[group];

	if (bitmaps.find(name) == bitmaps.end())
	{
		assert(false && "없는 키 - ResourceManager::GetBitmap");

		return nullptr;
	}

	return bitmaps[name];
}