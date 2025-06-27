#include "pch.h"
#include "ResourceManager.h"

#include <cassert>

using Microsoft::WRL::ComPtr;

ResourceManager& ResourceManager::Get()
{
	static ResourceManager s_instance;

	return s_instance;
}

HRESULT ResourceManager::Initialize(ComPtr<ID2D1DeviceContext7> deviceContext,
	const std::wstring& modulePath,
	const std::wstring& binaryFolderName,
	const std::wstring& resourceFolderName)
{
	m_d2d1DeviceContext = deviceContext;

	HRESULT hr{};

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory), (void**)m_wicImagingFactory.GetAddressOf());

	if (FAILED(hr))
	{
		return hr;
	}

	bool isWrongDirectory{ true };

	m_resourcePath = modulePath;

	size_t pos{ m_resourcePath.length() };

	while (pos != std::wstring::npos)
	{
		pos = m_resourcePath.rfind(L'\\', pos - 1);
		if (pos != std::wstring::npos)
		{
			std::wstring folderName{ m_resourcePath.substr(pos + 1) };
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
		return E_FAIL;
	}

	m_resourcePath += std::wstring(L"\\" + resourceFolderName + L"\\");

	return S_OK;
}

void ResourceManager::Release()
{
	m_bitmapResources.clear();

	m_wicImagingFactory = nullptr;
	m_d2d1DeviceContext = nullptr;
}

void ResourceManager::ReleaseResources()
{
	m_bitmapResources.clear();
}

std::shared_ptr<BitmapResource> ResourceManager::CreateBitmapResource(const std::wstring& filePath)
{
	const auto& iter{ m_bitmapResources.find(filePath) }; // 찾기
	if (iter != m_bitmapResources.end()) // 이미 있을 경우
	{
		if (!iter->second.expired()) // 만료되지 않았을 경우
		{
			return iter->second.lock(); // shared_ptr로 return
		}
	}

	// 없거나 만료되었을 경우
	std::shared_ptr<BitmapResource> newBitmapResource{ std::make_shared<BitmapResource>() };
	HRESULT hr{ newBitmapResource->CreateBitmap(m_d2d1DeviceContext, m_wicImagingFactory, m_resourcePath + filePath) };
	if (FAILED(hr))
	{
		assert(false && "BitmapResource::CreateBitmap 실패");

		return nullptr;
	}

	m_bitmapResources[filePath] = newBitmapResource;

	return newBitmapResource;
}