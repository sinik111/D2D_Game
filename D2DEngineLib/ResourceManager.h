#pragma once

class ResourceManager
{
private:
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2d1DeviceContext;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicImagingFactory;

	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> m_bitmaps;

	std::wstring m_resourcePath;

private:
	ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	~ResourceManager() = default;

private:
	static ResourceManager& Get();

public:
	static void Initialize(Microsoft::WRL::ComPtr<ID2D1DeviceContext7> deviceContext,
		const std::wstring& modulePath,
		const std::wstring& binaryFolderName,
		const std::wstring& resourceFolderName);
	static void Release();

	static void ReleaseResources();

public:
	static HRESULT LoadBitmapFromFile(const std::wstring name, const std::wstring& fileName);
	static Microsoft::WRL::ComPtr<ID2D1Bitmap1> GetBitmap(const std::wstring name);
};