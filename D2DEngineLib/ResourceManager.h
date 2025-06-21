#pragma once

class ResourceManager
{
private:
	using Bitmaps = std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap1>>;
	using BitmapGroups = std::unordered_map<std::wstring, Bitmaps>;

	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2d1DeviceContext;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicImagingFactory;

	BitmapGroups m_bitmapGroups;

	std::wstring m_resourcePath;

private:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	~ResourceManager() = default;

public:
	static ResourceManager& Get();

public:
	void Initialize(Microsoft::WRL::ComPtr<ID2D1DeviceContext7> deviceContext,
		const std::wstring& modulePath,
		const std::wstring& binaryFolderName,
		const std::wstring& resourceFolderName);
	void Release();

	void ReleaseResources(const std::wstring group);

public:
	HRESULT LoadBitmapFromFile(const std::wstring group, const std::wstring name, const std::wstring& fileName);
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> GetBitmap(const std::wstring group, const std::wstring name);
};