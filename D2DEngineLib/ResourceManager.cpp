#include "pch.h"
#include "ResourceManager.h"

#include <cassert>

using Microsoft::WRL::ComPtr;

ResourceManager::ResourceManager()
	: m_d2d1DeviceContext{ nullptr },
	m_wicImagingFactory{ nullptr }
{

}

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
	Get().m_d2d1DeviceContext = deviceContext;

	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(Get().m_wicImagingFactory), (void**)Get().m_wicImagingFactory.GetAddressOf());

	bool wrongDirectory = true;

	std::wstring& resourcePath = Get().m_resourcePath;

	resourcePath = modulePath;

	size_t pos = resourcePath.length();

	while (pos != std::wstring::npos)
	{
		pos = resourcePath.rfind(L'\\', pos - 1);
		if (pos != std::wstring::npos)
		{
			std::wstring folderName = resourcePath.substr(pos + 1);
			if (folderName == binaryFolderName)
			{
				resourcePath = resourcePath.substr(0, pos); // ���� �̸� ����
				wrongDirectory = false;
				break;
			}

			resourcePath = resourcePath.substr(0, pos);
		}
	}

	if (wrongDirectory)
	{
		return;
	}

	resourcePath += std::wstring(L"\\" + resourceFolderName + L"\\");

	return;
}

void ResourceManager::Release()
{
	Get().m_wicImagingFactory = nullptr;
	Get().m_d2d1DeviceContext = nullptr;
}

void ResourceManager::ReleaseResources()
{
	Get().m_bitmaps.clear();
}

HRESULT ResourceManager::LoadBitmapFromFile(const std::wstring name, const std::wstring& fileName)
{
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap1>>& bitmaps = Get().m_bitmaps;

	if (bitmaps.find(name) != bitmaps.end())
	{
		assert(false && "���� Ű ���� - ResourceManager::LoadBitmapFromFile");

		return E_FAIL;
	}

	ComPtr<IWICImagingFactory>& imagingFactory = Get().m_wicImagingFactory;
	ComPtr<IWICBitmapDecoder> decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter> converter;

	std::wstring path = Get().m_resourcePath + fileName;

	// ���ڴ� ����
	HRESULT hr = imagingFactory->CreateDecoderFromFilename(
		path.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr))
	{
		return hr;
	}

	// ù ������ ���
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr))
	{
		return hr;
	}

	// ���� ��ȯ�� ����
	hr = imagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr))
	{
		return hr;
	}

	// GUID_WICPixelFormat32bppPBGRA�� ��ȯ
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

	// Direct2D ��Ʈ�� �Ӽ� (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	ComPtr<ID2D1Bitmap1> bitmap;

	// DeviceContext���� WIC ��Ʈ�����κ��� D2D1Bitmap1 ����
	hr = Get().m_d2d1DeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, bitmap.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	bitmaps.emplace(name, bitmap);

	return hr;
}

ComPtr<ID2D1Bitmap1> ResourceManager::GetBitmap(const std::wstring name)
{
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap1>>& bitmaps = Get().m_bitmaps;

	if (bitmaps.find(name) == bitmaps.end())
	{
		assert(false && "���� Ű - ResourceManager::GetBitmap");

		return nullptr;
	}

	return bitmaps[name];
}