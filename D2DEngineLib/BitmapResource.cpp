#include "pch.h"
#include "BitmapResource.h"

using Microsoft::WRL::ComPtr;

HRESULT BitmapResource::CreateBitmap(const ComPtr<ID2D1DeviceContext7>& d2d1DeviceContext,
	const ComPtr<IWICImagingFactory>& wicImagingFactory, const std::wstring& filePath)
{
	ComPtr<IWICBitmapDecoder> decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter> converter;

	HRESULT hr{};

	// ���ڴ� ����
	hr = wicImagingFactory->CreateDecoderFromFilename(filePath.c_str(), nullptr,
		GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
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
	hr = wicImagingFactory->CreateFormatConverter(&converter);
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
	D2D1_BITMAP_PROPERTIES1 bmpProps{
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED))
	};

	// DeviceContext���� WIC ��Ʈ�����κ��� D2D1Bitmap1 ����
	hr = d2d1DeviceContext->CreateBitmapFromWicBitmap(converter.Get(), 
		&bmpProps, m_d2d1Bitmap.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	m_size = m_d2d1Bitmap->GetSize();

	return S_OK;
}

const D2D1_SIZE_F& BitmapResource::GetSize() const
{
	return m_size;
}

const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& BitmapResource::GetBitmap() const
{
	return m_d2d1Bitmap;
}