#include "pch.h"
#include "ResourceManager.h"

#include <cassert>
#include <fstream>

#include "json.hpp"

using Microsoft::WRL::ComPtr;
using nlohmann::json;

// UTF-8 std::string을 std::wstring으로 변환 (Windows API 기반)
static std::wstring utf8_to_wide(const std::string& utf8_str)
{
	if (utf8_str.empty())
	{
		return L"";
	}

	int count = MultiByteToWideChar(
		CP_UTF8,
		0,
		utf8_str.c_str(),
		(int)utf8_str.length(),
		nullptr,
		0
	);

	std::vector<wchar_t> wide_buf(count);

	MultiByteToWideChar(
		CP_UTF8,
		0,
		utf8_str.c_str(),
		(int)utf8_str.length(),
		wide_buf.data(),
		count
	);

	return std::wstring(wide_buf.data(), wide_buf.size());
}

// std::wstring을 UTF-8 std::string으로 변환 (Windows API 기반)
static std::string wide_to_utf8(const std::wstring& wide_str)
{
	if (wide_str.empty())
	{
		return "";
	}
	
	int count = WideCharToMultiByte(
		CP_UTF8,
		0,
		wide_str.c_str(),
		(int)wide_str.length(),
		nullptr,
		0,
		nullptr,
		nullptr
	);

	std::vector<char> utf8_buf(count);

	WideCharToMultiByte(
		CP_UTF8,
		0,
		wide_str.c_str(),
		(int)wide_str.length(),
		utf8_buf.data(),
		count,
		nullptr,
		nullptr
	);

	return std::string(utf8_buf.data(), utf8_buf.size());
}

namespace nlohmann
{
	template <>
	struct adl_serializer<std::wstring>
	{
		static void to_json(json& j, const std::wstring& str)
		{
			j = wide_to_utf8(str);
		}

		static void from_json(const json& j, std::wstring& str)
		{
			str = utf8_to_wide(j.get<std::string>());
		}
	};
} // namespace nlohmann

static void from_json(const json& j, Sprite& sprite)
{
	j.at("name").get_to(sprite.name);
	j.at("x").get_to(sprite.x);
	j.at("y").get_to(sprite.y);
	j.at("width").get_to(sprite.width);
	j.at("height").get_to(sprite.height);
	j.at("pivotX").get_to(sprite.pivotX);
	j.at("pivotY").get_to(sprite.pivotY);
}

static void from_json(const json& j, FrameInfo& frameinfo)
{
	j.at("sprite").get_to(frameinfo.spriteName);
	j.at("time").get_to(frameinfo.time);
}

static void from_json(const nlohmann::json& j, EventInfo& eventinfo)
{
	j.at("name").get_to(eventinfo.name);
	j.at("parameter").get_to(eventinfo.parameter);
	j.at("time").get_to(eventinfo.time);
}

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

	HRESULT hr;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory),
		(void**)m_wicImagingFactory.GetAddressOf()
	);

	if (FAILED(hr))
	{
		return hr;
	}

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
		return E_FAIL;
	}

	m_resourcePath += std::wstring(L"\\" + resourceFolderName + L"\\");

	return S_OK;
}

void ResourceManager::Update()
{
	m_resourceTimer += MyTime::DeltaTime();

	while (!m_shortCachedBitmapResources.empty())
	{
		if (m_shortCachedBitmapResources.front().first > m_resourceTimer)
		{
			break;
		}

		m_shortCachedBitmapResources.pop();
	}


	// Debug::Log(std::to_string(m_shortCachedBitmapResources.size()));
}

void ResourceManager::Release()
{
	ReleaseResources();

	m_wicImagingFactory = nullptr;
	m_d2d1DeviceContext = nullptr;
}

void ResourceManager::ReleaseResources()
{
	m_shortCachedBitmapResources = {};

	m_bitmapResources.clear();
	m_spriteSheets.clear();
	m_animationClips.clear();
}

std::wstring ResourceManager::GetResourcePath() const
{
	return m_resourcePath;
}

std::shared_ptr<BitmapResource> ResourceManager::CreateBitmapResource(const std::wstring& filePath)
{
	const auto& iter = m_bitmapResources.find(filePath); // 찾기
	if (iter != m_bitmapResources.end()) // 이미 있을 경우
	{
		if (!iter->second.expired()) // 만료되지 않았을 경우
		{
			m_shortCachedBitmapResources.push({ m_resourceTimer + 60.0f, iter->second.lock() });

			return iter->second.lock(); // shared_ptr로 return
		}
	}

	// 없거나 만료되었을 경우
	std::shared_ptr<BitmapResource> newBitmapResource = std::make_shared<BitmapResource>();
	HRESULT hr = newBitmapResource->CreateBitmap(
		m_d2d1DeviceContext,
		m_wicImagingFactory,
		m_resourcePath + filePath
	);

	if (FAILED(hr))
	{
		assert(false && "BitmapResource::CreateBitmap 실패");

		return nullptr;
	}

	m_bitmapResources[filePath] = newBitmapResource;

	m_shortCachedBitmapResources.push({ m_resourceTimer + 60.0f, newBitmapResource });

	return newBitmapResource;
}

std::shared_ptr<SpriteSheet> ResourceManager::CreateSpriteSheet(const std::wstring& filePath)
{
	const auto& iter = m_spriteSheets.find(filePath);
	if (iter != m_spriteSheets.end())
	{
		if (!iter->second.expired())
		{
			return iter->second.lock();
		}
	}

	std::shared_ptr<SpriteSheet> newSpriteSheet = std::make_shared<SpriteSheet>();
	HRESULT hr = LoadSpriteSheet(filePath, newSpriteSheet);
	if (FAILED(hr))
	{
		return nullptr;
	}

	m_spriteSheets[newSpriteSheet->name] = newSpriteSheet;

	return newSpriteSheet;
}

std::shared_ptr<AnimationClip> ResourceManager::CreateAnimationClip(const std::wstring& filePath,
	std::unordered_map<std::wstring, std::shared_ptr<SpriteSheet>>& spriteSheets)
{
	const auto& iter = m_animationClips.find(filePath);
	if (iter != m_animationClips.end())
	{
		if (!iter->second.expired())
		{
			return iter->second.lock();
		}
	}

	std::shared_ptr<AnimationClip> newAnimationClip = std::make_shared<AnimationClip>();
	HRESULT hr = LoadAnimationClip(filePath, newAnimationClip, spriteSheets);
	if (FAILED(hr))
	{
		return nullptr;
	}

	m_animationClips[filePath] = newAnimationClip;

	return newAnimationClip;
}

HRESULT ResourceManager::LoadSpriteSheet(const std::wstring& filePath, std::shared_ptr<SpriteSheet>& spriteSheet)
{
	std::ifstream inFile(m_resourcePath + filePath);
	if (inFile.is_open())
	{
		json j;
		inFile >> j;
		inFile.close();
		
		spriteSheet->name = j["name"].get<std::wstring>();
		spriteSheet->width = j["width"];
		spriteSheet->height = j["height"];
		spriteSheet->sprites = j["sprites"].get<std::vector<Sprite>>();

		for (int i = 0; i < spriteSheet->sprites.size(); ++i)
		{
			spriteSheet->spriteIndexMap[spriteSheet->sprites[i].name] = i;
		}
	}
	else
	{
		assert(false && L"json 파일 열기 실패");
		
		return E_FAIL;
	}

	return S_OK;
}

HRESULT ResourceManager::LoadAnimationClip(const std::wstring& filePath,
	std::shared_ptr<AnimationClip>& animationClip,
	std::unordered_map<std::wstring, std::shared_ptr<SpriteSheet>>& spriteSheets)
{
	std::ifstream inFile(m_resourcePath + filePath);
	if (inFile.is_open())
	{
		json j;
		inFile >> j;
		inFile.close();
		
		animationClip->name = j["name"].get<std::wstring>();
		animationClip->filePath = j["filePath"].get<std::wstring>();
		animationClip->isLoop = j["isLoop"];
		animationClip->duration = j["duration"];
		animationClip->frames = j["frames"].get<std::vector<FrameInfo>>();
		animationClip->events = j["events"].get<std::vector<EventInfo>>();

		for (auto& frame : animationClip->frames)
		{
			const auto& spriteSheet = spriteSheets[animationClip->filePath];
			const auto& iter = spriteSheets[animationClip->filePath]->spriteIndexMap.find(frame.spriteName);
			if (iter != spriteSheet->spriteIndexMap.end())
			{
				frame.spriteIndex = iter->second;
			}
			else
			{
				assert(false && L"json 없는 sprite name");
			}
		}
	}
	else
	{
		assert(false && L"json 파일 열기 실패");

		return E_FAIL;
	}

	return S_OK;
}
