#pragma once

#include "BitmapResource.h"

class ResourceManager
{
private:
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2d1DeviceContext;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicImagingFactory;

	std::unordered_map<std::wstring, std::weak_ptr<BitmapResource>> m_bitmapResources;
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
	HRESULT Initialize(Microsoft::WRL::ComPtr<ID2D1DeviceContext7> deviceContext,
		const std::wstring& modulePath,
		const std::wstring& binaryFolderName,
		const std::wstring& resourceFolderName);

	void Release();
	void ReleaseResources();

public:
	std::shared_ptr<BitmapResource> CreateBitmapResource(const std::wstring& filePath);
};