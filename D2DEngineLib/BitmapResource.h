#pragma once

class BitmapResource
{
private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2d1Bitmap{};

	D2D1_SIZE_F m_size{};

public:
	const D2D1_SIZE_F& GetSize() const;
	const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& GetBitmap() const;

private:
	HRESULT CreateBitmap(const Microsoft::WRL::ComPtr<ID2D1DeviceContext7>& d2d1DeviceContext,
		const Microsoft::WRL::ComPtr<IWICImagingFactory>& wicImagingFactory,
		const std::wstring& filePath);

	friend class ResourceManager;
};